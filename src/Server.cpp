#include <iostream>
#include "ServerMaster.h"
#include "Worker.h"

class RandomCardVec g_rcs;
int g_players;
int g_cardsets;
int * g_fds;                //An array to store all fds for sockets to client

bool g_next_flag;           //Stand for ready for next operation



int main(void)
{
    int cur_round;
    int max_round;
    int cur_player = 0;
    PLAYSTATE state = SHUFFLE_CARDS;

    

    

    ServerMaster master(g_players, g_cardsets, g_fds);
        
    

}
