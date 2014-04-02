#ifndef _WORKER_H
#define _WORKER_H

#include "MainWorker.h"

struct thread_arg{
    int id;                 //Starting from 0
    int num_players;
    int timeout;            //In seconds
    char* read_buf;
    char* write_buf;
};

class MainWorker;

class Worker{
public:
    Worker(int id = -1, int np = 0, int sfd= 0, MainWorker* mw = NULL);
    ~Worker();

public:
    void Set_writable();
    void Clear_writable();
    void Set_worker_flag(int worker_id);
    int SendBuf(char* buf, size_t sz);

private:
    Worker(const Worker&);
    const Worker& operator= (const Worker&);
    
private:
    int worker_id;
    MainWorker* p_main_worker;
    int num_player;
    int sock_fd;
    char* worker_flag;
    pthread_t worker_thread;
};

#endif 
