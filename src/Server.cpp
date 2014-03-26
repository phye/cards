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

enum PLAYSTATE{
    SHUFFLE_CARDS,
    DISPATCHING_CARDS,
    WAITING_PRIME,
    CHANGE_CARD,
    PLAYING,
    RECORD
}

int main(void)
{
    int cur_round;
    int max_round;
    int cur_player=0;
    //Prompt users for number of players and number of cardsets

    max_round = (54 * g_cardsets) / g_players;

    g_fds = new int [g_players];   

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
        switch(state){
            case SHUFFLE_CARDS:
                {
                    rcs = GenerateRandomCardVec();
                    state = DISPATCHING_CARDS;					
                    break;
                }

            case DISPATCHING_CARDS:
                {
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
                    if (/* 4*g_cardsets cards left */){
                        if(hasPrime)        //Set by worker thread
                        {
                            state = CHANGE_CARD;
                        }
                        else
                        {
                            state = WAITING_PRIME;
                        }
                    }
                    break;
                    //TODO: need to set banker based on if it is the first round
                }

            case WAITING_PRIME:
                {
                    //wait for somebody to claim
                    //if no body claims after a while, show all the left cards and choose a prime
                    prime = 
                        state = CHANGE_CARD;
                    //Fall through
                }

            case CHANGE_CARD:
                {
					// wait some time for other player to claim different prime
					
                    //send all the left cards to banker
                    //And wait for the banker to send cards back
                    state = PLAYING;
                    first_player = banker;
                    //Fall through
                }

            case PLAYING:
                { /* In playing game state */ 
                    
                    cur_player = first_player;
					do
					{
						//send request to cur_player to send cards
						//Judge if the the card by player is valid
						if(/* card valid */)
						{
							//record card for cur_player
							cur_player ++;
						}
						else
						{
							//NACK
							//TODO: we need to design special NACK for unsuccessful ShuaiPai, and should not continue for this case
							continue;
						}
					}while(cur_player != first_player);

					//compare cards and record score for this round
					// decide the first_player of the next round
                    if( /* Is last round */ )
                        state = RECORD;
                    else
                        break;
                }

            case RECORD:
                {
                    //Do some record
                    //Choose first player of next round

                    state = SHUFFLE_CARDS;
                    break;
                }
        }
    }

}
