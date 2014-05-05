#ifndef _WORKER_H
#define _WORKER_H

#include <sys/select.h>
#include <pthread.h>
#include <stdint.h>

#include "MainWorker.h"
#include "error.h"
#include "Card.h"
#include "CardSet.h"
#include "Frame.h"

#define BUF_LENGTH 50
#define BASE_CARD_NUM 8

class MainWorker;

class Worker{
public:
    Worker(int id = -1, int np = 0, int sfd = 0, int timeout =10, MainWorker* mw = NULL);
    ~Worker();

public: 
    int Get_worker_id() { return mi_worker_id; }
    int Get_num_players() { return mi_num_players; }
    int Get_sock_fd() { return mi_sock_fd; }
    int Get_time_out() { return mi_time_out; }
    MainWorker* Get_main_worker() { return mp_main_worker; }
    void Set_time_out(int timeout) { mi_time_out = timeout; }

public:
    //APIs For MainWorker 
    int Dispatch_card(const Card&);
    int Send_cards_to_swap(const CardSet&);    
    int Notify_prime(const Card&);              
    int Notify_banker(int banker_id);
    int Notify_card_swap();
    int Notify_card_send();
    int Notify_round_result(uint8_t winner_id, uint8_t pts);
    int Notify_set_result(uint8_t winner_id, uint8_t pts);

    //APIs for Worker
    int Ack_prime_claim(uint16_t ack_tag);
    int Nack_prime_claim(uint16_t ack_tag);
    int Ack_swap_card_data(uint16_t ack_tag);
    int Ack_snd_card_data(uint16_t ack_tag);
    int Nack_snd_card_data(uint16_t ack_tag);

    int Bcast_prime_claim(const Card&, int num);
    int Bcast_snd_card(const CardSet& cs);
    int Bcast_inval_snd_card(const CardSet& cs);

    friend void* worker_func(void* arg);

private:
    Worker(const Worker&);
    const Worker& operator= (const Worker&);

private:
    void Set_writable() { FD_SET(mi_sock_fd, &m_wset); }
    void Clear_writable() { FD_CLR(mi_sock_fd, &m_wset); }
    void Set_readable() { FD_SET(mi_sock_fd, &m_rset); }
    void Clear_readable() { FD_CLR(mi_sock_fd, &m_rset); }
    inline int Check_and_set_writing();  //atomic check and set writing
    inline int Clear_writing();

    void Set_worker_flag(int worker_id);
    //Clear all worker_flag except self
    void Clear_worker_flag();   

private:
    bool Send_buf(void* buf, size_t sz);
    bool Send_buf(int id, void* buf, size_t sz);
    bool Is_worker_flag_all_set();
    bool Bcast_to_others(void* buf, size_t sz);
    int Build_header(FrameType_t ft, uint16_t ack_tag, void* buf, size_t buf_len);
    bool Is_valid_ack(uint16_t ack_tag) { return ack_tag == ms_frame_num};

    void Set_ready_for_swap_card() { mb_ready_for_swap_card = true; }
    void Clear_ready_for_swap_card() { mb_ready_for_swap_card = false; }
    bool Is_ready_for_swap_card() { return mb_ready_for_swap_card; }
    void Set_ready_for_card() { mb_ready_for_card = true; }
    void Clear_ready_for_card() { mb_ready_for_card = false; }
    bool Is_ready_for_card() { return mb_ready_for_card; }
    
    int Ack_helper(FrameType_t ft, uint16_t ack_tag);
    int Notify_helper(FrameType_t ft);
    
private:
    short ms_frame_num;
    int mi_worker_id;
    int mi_num_players;
    int mi_sock_fd;
    int mi_time_out;
    MainWorker* mp_main_worker;
    char* mp_worker_flag;
    bool mb_ready_for_swap_card;
    bool mb_ready_for_card;
    bool mb_is_writing;
    pthread_t m_thread;
    pthread_mutex_t m_mtx;

private:
    uint8_t* mp_rbuf;
    uint8_t* mp_wbuf;

private:
    fd_set m_rset;
    fd_set m_wset;
};

#endif 
