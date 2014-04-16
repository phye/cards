#include "Worker.h"
#include "Frame.h"
#include "Card.h"
#include <stddef.h>
#include <string.h>
#include <sys/select.h>
#include <arpa/inet.h>


Worker::Worker(int id, int np, int sfd, int timeout, MainWorker* mw) 
    : mi_worker_id(id), mi_num_players(np), mi_sock_fd(sfd),
      mi_time_out(timeout), mp_main_worker(mw)
{
    ms_frame_num = 0;
    mp_worker_flag = new char[mi_num_players];
    memset(mp_worker_flag, 0, mi_num_players);
    mp_worker_flag[mi_worker_id] = 1;

    mp_rbuf = new char[BUF_LENGTH];
    mp_wbuf = new char[BUF_LENGTH];
    memset(mp_rbuf, 0, BUF_LENGTH);
    memset(mp_wbuf, 0, BUF_LENGTH);

    FD_ZERO(&m_rset);
    FD_ZERO(&m_wset);

    FD_SET(mi_sock_fd, &m_rset);

    pthread_create(&m_thread, NULL, worker_func, (void*) this);
}

Worker::~Worker()
{
    delete [] mp_worker_flag;
    delete [] mp_rbuf;
    delete [] mp_wbuf;
    pthread_exit((void*) 0);
}

void Worker::Set_writable()
{
    int sec = 0;
    //If fd is already set in wr set, we're already writing, wait until timeout
    while( FD_ISSET(mi_sock_fd, &m_wset) ) 
    {
        if (sec == mi_time_out)
            exit(SET_WRITABLE_TIMEOUT);
        sleep(1);
        sec ++;
    }
    FD_SET(mi_sock_fd, &m_wset);
}

void Worker::Set_worker_flag(int worker_id)
{
    Worker* worker = mp_main_worker->Get_worker(worker_id);
    //FIXME, public/private is class concept
    worker->mp_worker_flag[mi_worker_id] = 1;
}

void Worker::Clear_worker_flag()
{
    memset(mp_worker_flag, 0, mi_num_players);
    mp_worker_flag[mi_worker_id] = 1;
}

// Send_buf doesn't guarantee that buffer are sent and ACK are read 
// Worker need to set some flag to tell MainWorker that ACK are read
// Caller are responsible for free buf
bool Worker::Send_buf(char* buf, int sz)
{
    int sec = 0;
    while( FD_ISSET(mi_sock_fd, &m_wset) ) 
    {
        if (sec == mi_time_out)
            exit(SEND_BUF_TIMEOUT);
        sleep(1);
        sec ++;
    }

    memset(mp_wbuf, 0, BUF_LENGTH);
    memcpy(mp_wbuf, buf, sz);
    Set_writable();
    return true;
}

bool Worker::Send_buf(int id, char* buf, int sz)
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

bool Worker::Bcast_to_others(char* buf, int sz)
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
        sleep(1);
        sec ++;
    }

    return true;
}



void* worker_func(void* arg)
{
    Worker* pw= (Worker*) arg;
    MainWorker* pm = pw->Get_main_worker();
    int id = pw->Get_worker_id();
    int fd = pw->Get_sock_fd();
    struct timeval sel_tmout;
    sel_tmout.tv_sec = 0;
    sel_tmout.tv_nsec = 1000;   //select after every ms

    while(1) {
        int ret = select(fd+1, p_rset, p_wset, NULL, &sel_tmout);
        //FIXME: Rely on system to restart select when interrupted in select
        if (FD_ISSET(fd, p_wset)) {
            int wsz = sizeof(FrameHead_t) + 
                pw->mp_wbuf[offsetof(FrameHead_t, frm_len)];
            //sock_write is reliable 
            int nw = sock_write(fd, pw->mp_wbuf, wsz);
            memset(pw->mp_wbuf, 0, BUF_LENGTH);
            pw->Clear_writable();
        }
        if (FD_ISSET(fd, p_rset)) {
            //Read everything to pw->mp_rbuf;
            //Parse 
            FrameType_t ft = pw->mp_rbuf[offsetof(FrameHead_t, frm_type)];
            unsigned char data_len = pw->mp_rbuf[offsetof(FrameHead_t, frm_len)];
            char* payload = pw->mp_rbuf[sizeof(FrameHead_t)];
            switch(ft) {
                case DISPATCH_CARD_ACK:
                    {
                        //FIXME: update status name with X1n
                        if( pm->Get_current_state()
                                == DISPATCHING_CARD )
                            pm->Set_next_ready();
                        break;
                    }
                case CLAIM_PRIME:
                    {
                        if (data_len > 2) 
                            break;
                        else if (data_len == 2) {
                            if ( payload[0] != payload[1] )
                                break;
                        }
                        //TODO, add ctor of Card
                        Card prime(payload[0]); 
                        int ret = pm->Set_prime_card(prime, data_len);
                        if (ret == 0){
                            //Generate CLAIM_PRIME_ACK package and send to client
                            //Generate Bcast package and call Bcast
                        } else {
                            //Generate CLAIM_PRIME_NACK package and send to client
                        }
                        break;
                    }
                case CLAIM_PRIME_BCAST_ACK:
                    {
                        int snd_id = pw->rbuf[offsetof(FrameHead_t, frm_src)];
                        pm->Get_worker(snd_id)->Set_worker_flag(id);
                        break;
                    }
                case SWAP_CARD_NOTIF_ACK:
                    {
                        //Currently do nothing
                        //In the future, it may trigger sending starting count down 
                        //counter in other players
                        break;
                    }
                case SWAP_CARD_DATA:
                    {
                        if (data_len == BASE_CARD_NUM) {
                            CardSet base_cards(payload, data_len);
                            pm->Set_base_cards(base_cards);
                            //Generate SWAP_CARD_DATA_ACK package and send to client
                            pm->Set_next_ready();
                        }
                        else {
                            //TODO, add some error handling
                        }
                        break;
                    }
                case SND_NOTIF_ACK:
                    {
                        //Currently do nothing
                        //In the future, it may trigger sending starting count down 
                        //counter in other players
                        break;
                    }
                case SND_CARD:
                    {
                        CardSet snd_cards(payload, data_len);
                        CardSet* pcs = NULL;
                        if (pm->Is_valid_snd(id, snd_cards)){
                            pcs = &snd_cards;
                            pm->Update_card_set(id, *pcs);
                            //Generate SND_CARD_ACK and send to client
                        } else {
                            pcs = pm->Get_smallest_from_snd_cards(id, snd_cards);
                            pm->Update_card_set(id, *pcs);
                            //Generate SND_CARD_NACK and send to client
                            //SND_CARD_NACK should contain NACK + *pcs
                        }

                        pm->Update_card_set(id, *pcs);
                        
                        //Generate SND_CARD_BCAST package and call Bcast
                        //SND_CARD_BCAST should contain *pcs
                        if (pcs != &snd_cards)
                            delete pcs;

                        pm->Set_next_ready();
                        break;
                    }
                case ROUND_RESULT_NOTIF_ACK:
                    {
                        pm->Set_record_bcast_flag(id);
                        if (/* All workers are bcasted*/)
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

/*@Params: 
 * ft, Header method type
 * buf, the buffer to be filled, should be newed and deleted by caller
 * buf_len, length of buf
 */
    
int Worker::Generate_frame(FrameHead_t ft, short ack_tag, void* buf, size_t buf_len)
{
    if (ft < 0 || ft > FRAME_TYPE_MAX)
        return -1;

    FrameHead_t* head = (FrameHead_t*) buf;
    head->frm_magic_num = htonl(0xCAFEDADD);
    head->frm_src = mi_worker_id;
    head->frm_ver = 1;
    head->frm_type = (char) ft;
    head->frm_len = buf_len - sizeof(FrameHead_t);
    head->frm_pad1 = 0;
    head->frm_pad2 = htons(0);
    if (ft < ACK_START) 
        head->frm_tag = htons(ms_frame_num++) ;
    else
        head->frm_tag = htons(ack_tag);

    return 0;
}

int Worker::Dispatch_card(const Card& card)
{
    FrameType_t ft = DISPATCH_CARD;
    size_t buf_len = sizeof(FrameHead_t) + 1;
    void* buf = new char [buf_len];

    //TODO: Card.Get_char();
    buf[sizeof(FrameHead_t)] = card.Get_char();
    Generate_frame(ft, 0, buf, buf_len);
    Send_buf(buf, buf_len);
    delete [] buf;
}

int Worker::Banker_notify()
{
    FrameType_t ft = BANKER_NOTIF;
    size_t buf_len = sizeof(FrameHead_t);
    void* buf = new char [buf_len];

    Generate_frame(ft, 0, buf, buf_len);
    Send_buf(buf, buf_len);
    delete [] buf;
}

//FIXME: This function may not be necessary
int Worker::Swap_card_notify()
{
    FrameType_t ft = SWAP_CARD_NOTIF;
    size_t buf_len = sizeof(FrameHead_t);
    void* buf = new char [buf_len];

    Generate_frame(ft, 0, buf, buf_len);
    Send_buf(buf, buf_len);
    delete [] buf;
}

int Worker::Send_card_notify()
{
    FrameType_t ft = SND_CARD_NOTIF;
    size_t buf_len = sizeof(FrameHead_t);
    void* buf = new char [buf_len];

    Generate_frame(ft, 0, buf, buf_len);
    Send_buf(buf, buf_len);
    delete [] buf;
}
