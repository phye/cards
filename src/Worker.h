#ifndef _WORKER_H
#define _WORKER_H

#include <sys/select.h>
#include <pthread.h>
#include "MainWorker.h"
#include "error.h"

#define BUF_LENGTH 50
#define BASE_CARD_NUM 8

struct thread_arg{
    Worker* p_worker;
};

class MainWorker;

class Worker{
public:
    Worker(int id = -1, int np = 0, int sfd = 0, int timeout =10, MainWorker* mw = NULL);
    ~Worker();

public:
    void Set_writable();
    void Clear_writable() { FD_CLR(mi_sock_fd, &m_wset); }
    void Set_readable() { FD_SET(mi_sock_fd, &m_rset); }
    void Clear_readable() { FD_CLR(mi_sock_fd, &m_rset); }
    void Set_worker_flag(int worker_id);
    void Clear_worker_flag();   //Clear all worker_flag except self

    bool Send_buf(char* buf, size_t sz);
    bool Send_buf(int id, char* buf, size_t sz);

public: 
    int Get_worker_id() { return mi_worker_id; }
    int Get_num_players() { return mi_num_players; }
    int Get_sock_fd() { return mi_sock_fd; }
    int Get_time_out() { return mi_time_out; }
    MainWorker* Get_main_worker() { return mp_main_worker; }

    friend void* worker_func(void* arg);

public: //Just enable setting time out, other members should be inited in ctor
    void Set_time_out(int timeout) { mi_time_out = timeout; }

private:
    Worker(const Worker&);
    const Worker& operator= (const Worker&);
    bool Is_worker_flag_all_set();
    bool Is_valid_prime_claim();
    bool Bcast_to_workers(char* buf, size_t sz);
    
private:
    int mi_worker_id;
    int mi_num_players;
    int mi_sock_fd;
    int mi_time_out;
    MainWorker* mp_main_worker;
    char* mp_worker_flag;
    pthread_t m_thread;

private:
    char* mp_rbuf;
    char* mp_wbuf;

private:
    fd_set m_rset;
    fd_set m_wset;
};

#endif 
