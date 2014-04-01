#include <iostream>

class RandomCardVec g_rcs;
int g_players;
int g_cardsets;
int * g_fds;                //An array to store all fds for sockets to client

bool g_next_flag;           //Stand for ready for next operation

struct thread_arg{
    int id;                 //Starting from 0
    int num_players;
    int timeout;            //In seconds
    char* read_buf;
    char* write_buf;
};

void* thread_func(void* arg);

enum PLAYSTATE{
    SHUFFLE_CARDS,
    DISPATCHING_CARDS,
    WAITING_PRIME,
    CHANGE_CARD,
    PLAYING,
    RECORD,
    GAME_END
};

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

                    //Server should notify one player as the banker
                    /* 
                                    * server -> client: Hey, you're the banker, and here's your new card
                                    * client -> server: ACK
                                    * client -> server: Here's the changed card
                                    * server -> client: ACK
                                    */ 
                    //send all the left cards to banker
                    //And wait for the banker to send cards back
                    state = PLAYING;
                    first_player = banker;
                    //Fall through
                }

            case PLAYING:
                { 
                    /* In playing game state */ 
                    /* 
                                    * server -> client: Hey, you're the one to send card
                                    * client -> server: OK, here's my cards
                                    * server -> client: ACK FIXME: Is this necessary? //X1n: This is necessary, I think all card exchange action should be acked.
                                    */

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
                    if(/*larger than highest level*/)
                    {
                        state = GAME_END;
                    }
                    else
                    {
                        state = SHUFFLE_CARDS;
                    }
                    break;
                }

            case GAME_END:
                {
                    //Show the game result
                    //exit program or start another game by user's choice
                    break;
                }
        }
    }

}
