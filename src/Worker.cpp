#include "Worker.h"
#include "Frame.h"
#include <stddef.h>
#include <string.h>
#include <sys/select.h>

Worker::Worker(int id, int np, int sfd, int timeout, MainWorker* mw) 
    : mi_worker_id(id), mi_num_players(np), mi_sock_fd(sfd),
      mi_time_out(timeout), mp_main_worker(mw)
{
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
        return SendBuf(buf, sz);
    else {
        Worker* worker = mp_main_worker->Get_worker(id);
        return worker->SendBuf(buf,sz);
    }
}

bool Worker::Is_worker_flag_all_set()
{
    bool ret = true;
    for (int i=0; i != mi_num_players; i++)
        ret &= (bool) mp_worker_flag[i]; 
    return ret;
}

bool Worker::Bcast_to_workers(char* buf, int sz)
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

#if 0
static int worker_bcast(void * arg, char* buf)
{
    thread_arg* pargs = (thread_arg*) arg;
    int id = pargs->id;
    int num_players = pargs->num_players;
    char* wr_flags = g_worker_flag[id];
    memset(wr_flags, 0, pargs->num_players);
    g_worker_flag[id][id] = 1;

    //Copy everything on buf to other workers' write buffer
    //and make all other socket writable
    
    //TODO: We'd better put bcasting worker's id in the package header
    while ( /* g_worker_flag[] are not all 1 */ ) {
        if ( /* time out */ )
        {
            // return with error code
        }
    }

    //all other players have been brocasted successfully, return with success
    return 0;
}
#endif 

void* worker_func(void* arg)
{
    Worker* pw= (Worker*) arg;
    int id = pw->Get_worker_id();
    int fd = pw->Get_sock_fd();
    struct timeval sel_tmout;
    sel_tmout.tv_sec = 0;
    sel_tmout.tv_nsec = 1000;   //select after every ms

    while(1) {
        int ret = select(fd+1, p_rset, p_wset, NULL, &sel_tmout);
        //FIXME: now rely on system to restart select when interrupted in select
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

        }

        //select will clear readset if not readable, have to set the 
        //interest field again
        pw->Set_readable();
    }
}
void* thread_func(void* arg)
{
    thread_arg* pargs = (thread_arg*) arg;
    int id = pargs->id;
    int fd = g_fds[pargs->id];
    char* wr_buf = pargs->write_buf;

    while(1) {
        // Epoll/Select on fd
        {
            if (/*fd writable */) {
                //Write everything in pargs->writ_buf to fd
                
                //TODO: need to do some error handling
                
                //We're driven by other workers, need to tell others
                //TODO: need to change based on package header format
                if ( wr_buf[0] != id ){     
                    g_worker_flag[wr_buf[0]] = 1;
                }

                //Empty pargs->write_buf
                //Make fd unwritable
            }
            if ( /*fd readable */) {
                //Read everything to pargs->read_buf
                //Parsing pargs->read_buf 

                //Main is in DISPATCHING_CARDS
                if ( /* Is ACK from client */ ) {
                    next_flag = true;
                } else if ( /* Is CLAIMING Primes */ ) {
                    int rc = 0;
                    if ( /* Is valid Prime CLAIMING */ ){
                        //Store prime card in Server
                        
                        //Generate ACK package, copy to wr_buf
                        //then make own socket writable

                        //Generate brocatest package, then save it to buf
                        //Broadcast everything on buf to other workers
                        rc = worker_bcast(args, buf);
                        if (rc < 0)
                            exit(/*some error code*/);
                    } else { /* Invalid CLAIMING */ 
                        //Generate NACK package, copy to wr_buf
                        //then make own socket writable
                    }
                    continue;
                } else if ( /* CARD Rcvd from client for swapping*/ ) {
                    //Save cards received from client in server
                    //Update cardsets for banker

                    //Generate package based on cards to be swapped to the banker
                    //Copy to wr_buf, then make own socket writable

                    //We're not done yet, should wait for ACK for card swapping
                    continue;
                } else if ( /* CARD swapping ACK */ ) {
                    next_flag = true;
                } else if ( /* Card rcvd from client */ ) { //PLAYING
                    if ( /* Is valid card/cardSet */ ) {
                        int rc = 0;
                        //Update server's card set for corresponding client
                        //FIXME: is ACK from server necesasry ?
                        //Generate ACK, copy to wr_buf, and set socket writable

                        //Generate package to be brocasted to other workers
                        //and save them to buf
                        rc = worker_bcast(args, buf);
                        if (rc < 0)
                            exit(/*some error code*/);
                        //Brodcasting finished, main can continue
                        next_flag = true;
                    } else { //TODO: dealing with invalid cards

                    }
                }
            }
        }
    }
}
