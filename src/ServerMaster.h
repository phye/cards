#ifndef _SERVER_MASTER_H_
#define _SERVER_MASTER_H_

#include <vector>

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

#define LEVEL_GAP_DEFAULT   20
#define LEVEL_END           16


class ServerMaster{
    public: 
        //initialize all needed variables and environment for card in constructor, also need to create workers
        ServerMaster();
        ~ServerMaster();

        // Init environment and ready for play
        bool Init();
        void WaitPlayerReady();
        
        // Card playing
        void Reset();//reset all data
        void Run();
        void Shuffle();
        void DispatchCard();
        void WaitForPrime();
        void ExchangeCard();
        void PlayOneRound();
        void RecordScore();
        void RoundEnd();
        void GameEnd();

        // Getting info
        void GetScore(int* scoreContainer);
        int  GetCurrentSet();
        int  GetPlayingLevel(PLAYERNAME player);
        Card GetCurrentPrime();
        int  GetBanker(void);
        PLAYSTATE GetCurrentState();

        // Setting info
        bool ClaimPrime(Card claimingCard);
        void SetBanker(int newBanker);
        void SetLevelGap(int gap);

        void SetNextReady(int workerId);
        
        bool IsBanker(int player);

    private:
        bool IsLastHand();

    private:
        //TODO: change these card holders to vector, according to discussion with Ye.
        vector<Card> allCards;//the card stack, all cards after shuffle
        CardSet* usedCards;//need to initialize for each player in constructor, card played by one player
                            //Do we really need this? but we can still keep track of it
        CardSet* cardsInHand;//track the cards in player's hand to judge if there's illegal playing.
                             //but this could be done in Client, can remove this member
                             //!!!Server should track this too, considering ShuaiPai, or it may be rejected during broadcast?
        CardSet bottomCards;
        CardSet* cardPlayedInThisRound;//TODO: further consideration: ShuaiPai, how to store and how to compare, card played by one player in this hand
        Worker * workers;//intialize to proper number of workers

        int playerCount;
        int cardSetCount;
        int levelGap;//20 points or 40 points?

        PLAYSTATE currentState;
        int playerScore[2];
        int currentSet;
        int currentRound;//maybe we don't need this
        
        PLAYERNAME banker;
        PLAYERNAME firstPlayer;
        int playingLevel[2];//level0 for PLAYER1/3, level1 for PLAYER2/4
        Card currentPrime;

        int workerReadyFlag;

        int* fds;
};

/*******************************
worker functions needed:

1. workers[curPlayer].FetchCard(cardToDispatch);
2. workers[curPlayer].NeedPrime();
3. workers[curPlayer].GetPrime();
4. workers[curPlayer].GetBanker();
5. workers[banker].WaitChangeCard();
6. workers[curPlayer].PlayOneCard();

card functions:
1. cardPlayed[PLAYER_1].GetScore()
2. cardset.CopyToVector(allCards);
*******************************/

#endif
