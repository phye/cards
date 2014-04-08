#include <iostream>
#include "ServerMaster.h"
#include "Worker.h"

class RandomCardVec g_rcs;
int g_players;
int g_cardsets;
int * g_fds;                //An array to store all fds for sockets to client

bool g_next_flag;           //Stand for ready for next operation

typedef enum PLAY_STATE{
    SHUFFLE_CARDS,
    DISPATCHING_CARDS,
    WAITING_PRIME,
    CHANGE_CARD,
    PLAYING,
    RECORD,
    GAME_END
} PLAYSTATE;

int main(void)
{
    int cur_round;
    int max_round;
    int cur_player = 0;
    PLAYSTATE state = SHUFFLE_CARDS;

    g_player = 4; //default 4 players
    cout << "How many users?" << endl;
    cin >> g_players;

    g_cardsets = 2; //default 2 card sets
    cout << "How many set of cards?" << endl;
    cin >> g_cardsets;
    
    max_round = (54 * g_cardsets) / g_players;

    //TODO: create sockets.
    
    g_fds = new int [g_players]; 

    for(int idx = 0; idx < g_players; idx++)
    {
        g_fds[idx] = accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
        if(-1 != g_fds[idx])
        {
            //For now, we just kill the program itself on error, error handling will be developed later.
            exit -1;
        }
    }

    ServerMaster master(g_players, g_cardsets, g_fds);
        
    while(1) {
        switch(state){
            case SHUFFLE_CARDS:
                {
                    master.Shuffle();
                    state = DISPATCHING_CARDS;					
                    break;
                }

            case DISPATCHING_CARDS:
                {
                    master.DispatchCard()
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
                    master.ExchangeCard();
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

                    master.PlayOneRound()

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
                    master.RecordScore()
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
                    bool playAgain;
                    cout << "Wanner play again?" << endl;
                    cin >> playAgain;

                    if (playAgain)
                    {
                        master.Reset()
                        state = SHUFFLE_CARDS;
                    }
                        
                    break;
                }
        }
    }

}
