#ifndef _SERVER_MASTER_H_
#define _SERVER_MASTER_H_

#include <vector>
using std::vector;


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
    SET_END,
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
        bool ClaimPrime(CardSet claimingCard, int workerID);
        void SetBanker(int newBanker);
        void SetLevelGap(int gap);

        void SetNextReady(int workerId);
        void ReturnBottomCard(CardSet returnedCard);
        bool IsValidSend(int workerId, CardSet cards);
        void SendCard(int workerId, CardSet cards);
        
        bool IsBanker(PLAYERNAME player);

    private:
        void WaitWorkerSetReady(int workerID);
        void WaitWorkerSetReady();
        bool IsLastRound();
        bool SanityCheck(CardSet cards);

    private:
        vector<Card> allCards(108, CARD_INVALID_VAL);//the card stack, all cards after shuffle
        vector<CardSet> usedCards(MAX_PLAYER_COUNT);//need to initialize for each player in constructor, card played by one player
                            //Do we really need this? but we can still keep track of it
        vector<CardSet> cardsInHand(MAX_PLAYER_COUNT);//track the cards in player's hand to judge if there's illegal playing.
                             //but this could be done in Client, can remove this member
                             //!!!Server should track this too, considering ShuaiPai, or it may be rejected during broadcast?
        CardSet bottomCards;
        vector<CardSet> cardPlayedInThisRound(MAX_PLAYER_COUNT);//TODO: further consideration: ShuaiPai, how to store and how to compare, card played by one player in this hand
        vector<Worker> workers(MAX_PLAYER_COUNT);//intialize to proper number of workers

        int playerCount;
        int cardSetCount;
        int levelGap;//20 points or 40 points?

        PLAYSTATE currentState;
        vector<int> playerScore(2, 0);
        int currentSet;
        int currentRound;//maybe we don't need this
        
        PLAYERNAME banker;
        PLAYERNAME firstPlayer;
        vector<int> playingLevel(2, 2);//level0 for PLAYER1/3, level1 for PLAYER2/4
        Card currentPrime;
        bool doubleClaim;//indicate if currentPrime is claimed by double card

        int workerReadyFlag;//Ready Flag: A 1 on corresponding bit means not ready, set ready will clear the bit

        vector<int> fds(MAX_PLAYER_COUNT);
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
