#ifndef _WORKER_H
#define _WORKER_H

#include <sys/select.h>
#include <pthread.h>
#include "MainWorker.h"

#define BUF_LENGTH 50
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
    void Clear_writable();
    void Set_worker_flag(int worker_id);
    int SendBuf(char* buf, size_t sz);

public: 
    int Get_worker_id() { return mi_worker_id; }
    int Get_num_players() { return mi_num_players; }
    int Get_sock_fd() { return mi_sock_fd; }
    int Get_time_out() { return mi_time_out; }
    MainWorker* Get_main_worker() { return mp_main_worker; }

public: //Just enable setting time out, other members should be inited in ctor
    void Set_time_out(int timeout) { mi_time_out = timeout; }

private:
    Worker(const Worker&);
    const Worker& operator= (const Worker&);
    
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
