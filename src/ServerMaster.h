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
} PLAYERNAME;

typedef enum PLAY_STATE{
    SHUFFLE_CARDS,
    DISPATCHING_CARDS,
    WAITING_PRIME,
    CHANGE_CARD,
    PLAYING,
    RECORD_SCORE,
    ROUND_END,
    GAME_END
} PLAYSTATE;

#define LEVEL_GAP_DEFAULT 20

class ServerMaster{
    public: 
        //initialize all needed variables and environment for card in constructor, also need to create workers
        ServerMaster(int nPlayers, int nCardSets);
        ~ServerMaster();

        // Init environment and ready for play
        bool Init();
        bool WaitPlayerReady();
        
        // Card playing
        void Reset();//reset all data
        void Run();
        void Shuffle(CardSet cardset);
        void DispatchCard();
        void WaitForPrime();
        void ExchangeCard();
        void PlayOneLoop();
        void RecordScore();
        void RoundEnd();
        void GameEnd();

        // Getting info
        void GetScore(int* scoreContainer);
        int  GetCurrentRound();
        int  GetPlayingLevel(PLAYERNAME player);
        Card GetCurrentPrime();
        int  GetCurrentRound();
        int  GetBanker(void);
        PLAYSTATE GetCurrentState();

        // Setting info
        bool ClaimPrime(Card claimingCard);
        void SetBanker(int newBanker);
        void SetLevelGap(int gap);
        
        bool IsBanker(int player);

    private:
        bool IsLastLoop();

    private:
        //TODO: change these card holders to vector, according to discussion with Ye.
        CardSet remainingCards;
        CardSet* usedCards;//need to initialize for each player in constructor
        CardSet bottomCards;
        Worker * workers;//intialize to proper number of workers

        int playerCount;
        int cardSetCount;
        int levelGap;//20 points or 40 points?

        PLAYSTATE currentState;
        int playerScore[2];
        int currentRound;
        
        PLAYERNAME banker;
        PLAYERNAME firstPlayer;
        int playingLevel[2];//level0 for PLAYER1/3, level1 for PLAYER2/4
        Card currentPrime;
}

/*******************************
worker functions needed:

1. workers[curPlayer].FetchCard(cardToDispatch);
2. workers[curPlayer].NeedPrime();
3. workers[curPlayer].GetPrime();
4. workers[curPlayer].GetBanker();

*******************************/

#endif
