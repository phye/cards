#ifndef _SERVER_MASTER_H_
#define _SERVER_MASTER_H_

class ServerMaster{
    public: 
        //initialize all needed variables and environment for card in constructor, also need to create workers
        ServerMaster(int nPlayers, int nCardSets);
        ~ServerMaster();

        void GetScore(int* scoreContainer);
        void Reset();//reset all data
        void Shuffle(CardSet cardset);
        void DispatchCard();
        void ExchangeCard();
        void PlayOneRound();
        void RecordScore();
        void SetBanker(int newBanker);
        int GetBanker(void);
        bool IsBanker(int player);

    private:
//        CardSet allCard;
        CardSet remainingCards;
        CardSet* usedCards;//need to initialize for each player in constructor
        CardSet bottomCards;
        int* playerScore;
        int banker;
        
}


#endif
