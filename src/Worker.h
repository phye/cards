#ifndef _WORKER_H
#define _WORKER_H

struct thread_arg{
    int id;                 //Starting from 0
    int num_players;
    int timeout;            //In seconds
    char* read_buf;
    char* write_buf;
};


#endif 
