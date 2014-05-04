#include <stddef.h>
#include <string.h>
#include <sys/select.h>
#include <arpa/inet.h>

#include "Worker.h"
#include "Frame.h"
#include "Card.h"
#include "Util.h"

Worker::Worker(int id, int np, int sfd, int timeout, MainWorker* mw) 
    : mi_worker_id(id), mi_num_players(np), mi_sock_fd(sfd),
      mi_time_out(timeout), mp_main_worker(mw)
{
    ms_frame_num = 0;
    mp_worker_flag = new char[mi_num_players];
    memset(mp_worker_flag, 0, mi_num_players);
    mp_worker_flag[mi_worker_id] = 1;
    mb_ready_for_swap_card = false;
    mb_ready_for_card = false;
    mb_is_writing = false;

    mp_rbuf = new uint8_t [BUF_LENGTH];
    mp_wbuf = new uint8_t [BUF_LENGTH];
    memset(mp_rbuf, 0, BUF_LENGTH);
    memset(mp_wbuf, 0, BUF_LENGTH);

    FD_ZERO(&m_rset);
    FD_ZERO(&m_wset);

    FD_SET(mi_sock_fd, &m_rset);

    pthread_mutex_init(&m_mtx, NULL);
    pthread_create(&m_thread, NULL, worker_func, (void*) this);
}

Worker::~Worker()
{
    delete [] mp_worker_flag;
    delete [] mp_rbuf;
    delete [] mp_wbuf;
    pthread_exit((void*) 0);
}

int Worker::Check_and_set_writing()
{
    pthread_mutex_lock(&m_mtx);
    if( mb_is_writing ) {
        pthread_mutex_unlock(&m_mtx);
        return ALREADY_WRITING;
    } else {
        mb_is_writing = true;
        pthread_mutex_unlock(&m_mtx);
        return 0;
    }
}

int Worker::Clear_writing()
{
    //FIXME: Should I add mutex here?
    mb_is_writing = false;
    return 0;
}

void Worker::Set_worker_flag(int worker_id)
{
    Worker* pw = mp_main_worker->Get_worker(worker_id);
    //FIXME, public/private is class concept
    //No need to add lock here
    pw->mp_worker_flag[mi_worker_id] = 1;
}

void Worker::Clear_worker_flag()
{
    memset(mp_worker_flag, 0, mi_num_players);
    mp_worker_flag[mi_worker_id] = 1;
}

// Send_buf doesn't guarantee that buffer are sent and ACK are read 
// Worker need to set some flag to tell MainWorker that ACK are read
// Caller are responsible for free buf
bool Worker::Send_buf(void* buf, int sz)
{
    int retry = 0;
    while( Check_and_set_writing() ){
        if( retry > 50 )
            exit(SEND_BUF_TIMEOUT);
        retry ++;
        //TODO: sleep for 20 ms
    }

    memset(mp_wbuf, 0, BUF_LENGTH);
    memcpy(mp_wbuf, buf, sz);
    Set_writable();
    return true;
}

bool Worker::Send_buf(int id, void* buf, int sz)
{
    if ( id == mi_worker_id )
        return Send_buf(buf, sz);
    else {
        Worker* worker = mp_main_worker->Get_worker(id);
        return worker->Send_buf(buf,sz);
    }
}

bool Worker::Is_worker_flag_all_set()
{
    bool ret = true;
    for (int i=0; i != mi_num_players; i++)
        ret &= (bool) mp_worker_flag[i]; 
    return ret;
}

//ATTENTION: This is blocking.
bool Worker::Bcast_to_others(void* buf, int sz)
{
    int sec = 0;
    Clear_worker_flag();
    for (int i = 0; i != mi_num_players; i++)
    {
        if (i == mi_worker_id)
            continue;
        Send_buf(i, buf, sz);
    }

    while ( !Is_worker_flag_all_set() ) {
        if (sec == mi_time_out)
            exit(BCAST_BUF_TIMEOUT);
        //TODO: Sleep too long
        //sleep for 20 ms maybe enough
        sec ++;
    }

    return true;
}

/*@Params: 
 * ft, Header method type
 * buf, the buffer to be filled, should be newed and deleted by caller
 * buf_len, length of buf
 *
 *@Note:
 * ms_frame_num will ++ in generating NonACK&NonBcast headers
 */
    
int Worker::Build_header(FrameHead_t ft, uint16_t ack_tag, void* buf, size_t buf_len)
{
    if (ft < 0 || ft > FRAME_TYPE_MAX)
        return -1;

    FrameHead_t* head = (FrameHead_t*) buf;
    head->frm_magic_num = htonl(0xCAFEDADD);
    head->frm_src = mi_worker_id;
    head->frm_ver = 1;
    head->frm_type = (uint8_t) ft;
    head->frm_len = buf_len - sizeof(FrameHead_t);
    head->frm_pad1 = 0;
    head->frm_pad2 = htons(0);
    if (ft < ACK_START) {
        head->frm_tag = htons(ms_frame_num);
        if(ft < BCAST_START)
            ms_frame_num++;
        //We will not inc ms_frame_num in BCAST package, since they're sent to other
        //workers
    }
    else
        head->frm_tag = htons(ack_tag);

    return 0;
}

int Worker::Dispatch_card(const Card& card)
{
    FrameType_t ft = DISPATCH_CARD;
    size_t buf_len = sizeof(FrameHead_t) + 1;
    uint8_t* buf = new uint8_t [buf_len];

    //TODO: Card.Get_char();
    buf[sizeof(FrameHead_t)] = card.Get_char();
    Build_header(ft, 0, buf, buf_len);
    Send_buf(buf, buf_len);
    delete [] buf;
    return 0;
}

int Worker::Send_cards_to_swap(const CardSet& cs)
{
    FrameType_t ft = SWAP_CARD_DATA_SVR;
    size_t buf_len = sizeof(FrameHead_t) + cs.Size();
    uint8_t* buf = new uint8_t [buf_len];
    uint8_t* payload = buf + sizeof(FrameHead_t);
    
    cs.Get_char_array(payload, buf_len - sizeof(FrameHead_t));

    Build_header(ft, 0, buf, buf_len);  //My chest hurt at 22:10 04-May-14
    Send_buf(buf, buf_len);
    delete [] buf;
    return 0;
}

//This function will only be called when nobody claim for prime
//during dispathing card period, in which we have to decide the
//prime from bottom cards
int Worker::Notify_prime(const Card& card)
{
    FrameType_t ft = PRIME_NOTIF;
    size_t buf_len = sizeof(FrameHead_t) + 1;
    uint8_t* buf = new uint8_t [buf_len];

    buf[sizeof(FrameHead_t)] = card.Get_char();

    Build_header(ft, 0, buf, buf_len);
    Send_buf(buf, buf_len);
    delete [] buf;
    return 0;
}

int Worker::Notify_helper(FrameHead_t ft)
{
    size_t buf_len = sizeof(FrameHead_t);
    uint8_t* buf = new uint8_t [buf_len];

    Build_header(ft, 0, buf, buf_len);
    Send_buf(buf, buf_len);
    delete [] buf;
    return 0;
}

int Worker::Notify_banker()
{
    //TODO: Should notify other workers who's banker
    return Notify_helper(BANKER_NOTIF);
}

//FIXME: This function may not be necessary
int Worker::Notify_card_swap()
{
    //TODO: Server should send card first
    return Notify_helper(SWAP_CARD_NOTIF);
}

int Worker::Notify_card_send()
{
    FrameType_t ft = SND_CARD_NOTIF;
    return Notify_helper(SND_CARD_NOTIF);
}

int Worker::Notify_round_result(uint8_t winner_id, uint8_t pts)
{
    FrameType_t ft = ROUND_RESULT_NOTIF;
    size_t buf_len = sizeof(FrameHead_t) + 2;
    uint8_t* buf = new uint8_t [buf_len];
    
    buf[sizeof(FrameHead_t)] = winner_id;
    buf[sizeof(FrameHead_t) + 1] = pts;

    Build_header(ft, 0, buf, buf_len);
    Send_buf(buf, buf_len);
    delete [] buf;
    return 0;
}

int Worker::Notify_set_result(uint8_t winner_id, uint8_t pts)
{
    FrameType_t ft = SET_RESULT_NOTIF;
    size_t buf_len = sizeof(FrameHead_t) + 2;
    uint8_t* buf = new uint8_t [buf_len];
    
    buf[sizeof(FrameHead_t)] = winner_id;
    buf[sizeof(FrameHead_t) + 1] = pts;

    Build_header(ft, 0, buf, buf_len);
    Send_buf(buf, buf_len);
    delete [] buf;
    return 0;
}

int Worker::Ack_helper(FrameHead_t ft, uint16_t ack_tag)
{
    size_t buf_len = sizeof(FrameHead_t);
    uint8_t * buf = new uint8_t [buf_len];
    
    Build_header(ft, ack_tag, buf, buf_len);
    Send_buf(buf, buf_len);
    delete [] buf;
    return 0;
}

int Worker::Ack_prime_claim(uint16_t ack_tag)
{
    return Ack_helper(CLAIM_PRIME_ACK, ack_tag);
}

int Worker::Nack_prime_claim(short ack_tag)
{
    return Ack_helper(CLAIM_PRIME_NACK, ack_tag);
}

int Worker::Ack_swap_card_data(uint16_t ack_tag)
{
    return Ack_helper(SWAP_CARD_DATA_ACK, ack_tag);
}

int Worker::Ack_snd_card_data(uint16_t ack_tag)
{
    return Ack_helper(SND_CARD_DATA_ACK, ack_tag);
}

int Worker::Nack_snd_card_data(uint16_t ack_tag)
{
    return Ack_helper(SND_CARD_DATA_NACK, ack_tag);
}

int Worker::Bcast_prime_claim(const Card& prime, int num)
{
    FrameType_t ft = CLAIM_PRIME_BCAST;
    size_t buf_len = sizeof(FrameHead_t) + num;
    uint8_t* buf = new uint8_t [buf_len];
    uint8_t* payload = buf + sizeof(FrameHead_t);

    //TODO: Implement Card.Get_char() 
    for (int i=0; i< num; i++)
       payload[i] = prime.Get_char(); 

    Build_header(ft, 0, buf, buf_len);
    Bcast_to_others(buf, buf_len);
    return 0;
}

int Worker::Bcast_snd_card(const CardSet& cs)
{
    FrameHead_t ft = SND_CARD_DATA_BCAST;
    size_t buf_len = sizeof(FrameHead_t) + cs.Size();
    uint8_t* buf = new uint8_t [buf_len];
    uint8_t* payload = buf + sizeof(FrameHead_t);
    
    //TODO: Implement CardSet.Get_char_array()
    cs.Get_char_array(payload, buf_len-sizeof(FrameHead_t));

    Build_header(ft, 0, buf, buf_len);
    Bcast_to_others(buf,buf_len);
    return 0;
}

int Worker::Bcast_inval_snd_card(const CardSet& cs)
{
    FrameHead_t ft = SND_CARD_INVAL_DATA_BCAST;
    size_t buf_len = sizeof(FrameHead_t) + cs.Size();
    uint8_t* buf = new uint8_t [buf_len];
    uint8_t* payload = buf + sizeof(FrameHead_t);
    
    //TODO: Implement CardSet.Get_char_array()
    cs.Get_char_array(payload, payload-buf);

    Build_header(ft, 0, buf, buf_len);
    Bcast_to_others(buf,buf_len);
    return 0;
}

void* worker_func(void* arg)
{
    Worker* pw= (Worker*) arg;
    MainWorker* pm = pw->Get_main_worker();
    int id = pw->Get_worker_id();
    int fd = pw->Get_sock_fd();
    struct timeval sel_tmout;
    sel_tmout.tv_sec = 0;
    sel_tmout.tv_nsec = 20000;   //select after every 20 ms

    while(1) {
        int ret = select(fd+1, p_rset, p_wset, NULL, &sel_tmout);
        //FIXME: Rely on OS to restart select when interrupted in select
        if (FD_ISSET(fd, p_wset)) {
            int wsz = sizeof(FrameHead_t) + 
                pw->mp_wbuf[offsetof(FrameHead_t, frm_len)];
            //sock_write is reliable 
            int nw = sock_write(fd, pw->mp_wbuf, wsz);
            memset(pw->mp_wbuf, 0, BUF_LENGTH);
            pw->Clear_writing();
            pw->Clear_writable();
        }
        if (FD_ISSET(fd, p_rset)) {
            //Read header first
            int nr = sock_read(fd, pw->mp_rbuf, sizeof(FrameHead_t));
            if (nr != sizeof(FrameHead_t))
                exit(READ_INCOMPLETE);
            uint32_t* magic = (uint32_t*) pw->mp_rbuf + 
                offsetof(FrameHead_t, frm_magic_num);
            if (*magic != 0xCAFEDADD)
                exit(INVALID_PACKAGE);
            
            FrameType_t ft = pw->mp_rbuf[offsetof(FrameHead_t, frm_type)];
            uint8_t data_len = pw->mp_rbuf[offsetof(FrameHead_t, frm_len)];
            uint8_t* payload = pw->mp_rbuf[sizeof(FrameHead_t)];
            uint16_t ack_tag = pw->mp_rbuf[offsetof(FrameHead_t, frm_tag)];

            //Time to read body
            nr = sock_read(fd, payload, data_len);
            if (nr != data_len)
                exit(READ_INCOMPLETE);
            //TODO: CRC check

            switch(ft) {
                case DISPATCH_CARD_ACK:
                    {
                        //TODO: Add state check for MainWorker
                        if( pw->Is_valid_ack(ack_tag) )
                            pm->Set_next_ready();
                        break;
                    }
                case CLAIM_PRIME:
                    {
                        //TODO: Add state check for MainWorker
                        if (data_len > 2) 
                            break;
                        else if (data_len == 2) {
                            if ( payload[0] != payload[1] )
                                break;
                        }
                        //TODO, add ctor of Card, Card(char);
                        Card prime(payload[0]); 
                        int ret = pm->Set_prime_card(prime, data_len);
                        if (ret == 0){
                            pw->Ack_prime_claim(ack_tag);
                            pw->Bcast_prime_claim(prime, data_len);
                        } else {
                            pw->Nack_prime_claim(ack_tag);
                        }
                        break;
                    }
                case CLAIM_PRIME_BCAST_ACK:
                    {
                        //TODO: Add state check for MainWorker
                        //TODO: need to add validity check for BCAST_ACK
                        int snd_id = pw->rbuf[offsetof(FrameHead_t, frm_src)];
                        pm->Get_worker(snd_id)->Set_worker_flag(id);
                        break;
                    }
                case BANKER_NOTIF_ACK:
                    {
                        //FIXME: Is BANKER_NOTIF_ACK redundent? 
                        //Maybe SWAP_CARD_NOTIF is enough
                        
                        //Currently do nothing
                        break;
                    }
                case SWAP_CARD_NOTIF_ACK:
                    {
                        //TODO: Add state check for MainWorker
                        if (pw->Is_valid_ack(ack_tag)) {
                            pw->Set_ready_for_swap_card();
                            pm->Set_next_ready();
                        }
                        //TODO: It may trigger sending starting count down 
                        //counter in other players
                        break;
                    }
                    //TODO: Add processing for NACKs for other package
                case SWAP_CARD_SVR_ACK:
                    {
                    }
                case SWAP_CARD_DATA:
                    {
                        //TODO: Add state check for MainWorker
                        if(! pw->Is_ready_for_swap_card()){
                            //TODO: Log sth
                            break;
                        }
                        if (data_len == BASE_CARD_NUM) {
                            CardSet base_cards(payload, data_len);
                            pw->Ack_swap_card_data(ack_tag);
                            pm->Set_base_cards(base_cards);
                            pm->Set_next_ready();
                            pw->Clear_ready_for_swap_card();
                        }
                        else {
                            //TODO, add some error handling and log sth
                        }
                        break;
                    }
                case SND_CARD_NOTIF_ACK:
                    {
                        //TODO: Add state check for MainWorker
                        if (pw->Is_valid_ack(ack_tag))
                            pw->Set_ready_for_card();
                        //TODO: it may trigger sending starting count down 
                        //counter in other players
                        break;
                    }
                case SND_CARD_DATA:
                    {
                        //TODO
                        if (! pw->Is_ready_for_card() ) {
                            //TODO: Log sth
                            break;
                        }
                        CardSet snd_cards(payload, data_len);
                        // Directly call Mainwoker's function
                        if (pm->Is_valid_snd(id, snd_cards)){
                            pm->Update_card_set(id, snd_cards);
                            pw->Ack_snd_card_data(ack_tag);
                            pw->Bcast_snd_card(snd_cards);
                            pw->Clear_ready_for_card();
                            pm->Set_next_ready();
                        } else {
                            pw->Nack_snd_card_data(ack_tag);
                            pw->Bcast_inval_snd_card(snd_cards);
                            //On getting SND_CARD_DATA_NACK, client must send
                            //smallest card in snd_cards automatically
                        }
                        break;
                    }
                case ROUND_RESULT_NOTIF_ACK:
                    {
                        pm->Set_next_ready();
                        break;
                    }
                case SET_RESULT_NOTIF_ACK:
                    {
                        pm->Set_next_ready();
                        break;
                    }
                default:
                    {
                        //Invalid card received, log sth
                    }
            }
        }

        //select will clear readset if not readable, have to set the 
        //interest field again
        pw->Set_readable();
    }
}
