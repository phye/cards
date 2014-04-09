#ifndef _SERVER_MASTER_H_
#define _SERVER_MASTER_H_

typedef enum PLAYER_NAME
{
    PLAYER_1,
    PLAYER_2,
    PLAYER_3,
    PLAYER_4,
    MAX_PLAYER_COUNT,
    PLAYER_NONE = 0xFF
}PLAYERNAME;


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

        int playerCount;
        int cardSetCount;
        int levelUpGrade;//20 points or 40 points?

        int* playerScore;
        int currentRound;
        
        PLAYERNAME banker;
        PLAYERNAME firstPlayer;
        int playingLevel[2];//level0 for PLAYER1/3, level1 for PLAYER2/4
        Card currentPrime;
}


#endif
