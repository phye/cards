#include <iostream>
#include "ServerMaster.h"
#include "Worker.h"

/*
class RandomCardVec g_rcs;
int g_players;
int g_cardsets;
int * g_fds;                //An array to store all fds for sockets to client

bool g_next_flag;           //Stand for ready for next operation
*/


int main(void)
{
    ServerMaster master;
    master.Init();
    master.Run();

    cout << "Game end.." << endl;
}
