#include <iostream>

class RandomCardVec g_rcs;
int g_players;
int g_cardsets;
int * g_fds; //An array to store all fds for sockets to client

bool g_next_flag;     //Stand for ready for next operation

struct thread_arg{
    int id;
    char* read_buf;
    char* write_buf;
};

void thread_func(void* arg)
{
    thread_arg* pargs = (thread_arg*) arg;
    int fd = g_fds[id];

    while(1) {
        // Epoll/Select on fd
        {
            if (/*fd writable */) {
                //Write everything in pargs->writ_buf to fd

                //Empty pargs->write_buf
                //Make fd unwritable
                //Make current status CARD_SENT

                //FIXME: How to deal Claiming Prime ACK?
            }
            if ( /*fd readable */) {
                //Read everything to pargs->read_buf
                //Parsing pargs->read_buf 

                if ( /* Is request for Prime */ ){
                    //Take action needed to SetPrime
                    continue ;
                }

                if ( /* current status is CARD_SENT */ )
                    if ( /* Is the correct ACK */ )
                        g_next_flag = true;
            }
        }
        if ( /* current status is CARD_SENT */ )
            //Reset timer for select/poll to count for timeout
    }
}

int main(void)
{
    int cur_round;
    int max_round;
    int cur_player=0;
    //Prompt users for number of players and number of cardsets
    
    max_round = (54 * g_cardsets) / g_players;

    g_fds = new int [g_players];
    rcs = GenerateRandomCardVec();

    g_fds[0] = accept(...);
    g_fds[1] = accept(...);
    g_fds[2] = accept(...);
    g_fds[3] = accept(...);

    //Allocate space for receiving and sending buffers of each thread
    
   pthread_create(..., thread_func, (void*) thread_arg_1); 
   pthread_create(..., thread_func, (void*) thread_arg_2); 
   pthread_create(..., thread_func, (void*) thread_arg_3); 
   pthread_create(..., thread_func, (void*) thread_arg_4); 

   while(1) {
       if ( /* In dispatching state */ ){
           while( cur_player != g_players ){
               next_flag = false;
               //Get one card from g_rcs

               //Generate header and body for the card and save them to write_buf accordingly 

               //Make socket writable, the corresponding thread will finish rest of the job 
               //And set next_flag

               while (! next_flag ) {}
               cur_player ++;
           }
           cur_player = 0;
       }
       else { /* In playing game state */ 

       }
   }
}
