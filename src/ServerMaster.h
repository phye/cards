#ifndef _SERVER_MASTER_H_
#define _SERVER_MASTER_H_

class ServerMaster{
    public: 
        //initialize all needed variables and environment for card in constructor, also need to create workers
        ServerMaster(int nPlayers, int nCardSets);
        ~ServerMaster();

        //maintain the card play process
        void CardPlaying();

    private:
}


#endif
