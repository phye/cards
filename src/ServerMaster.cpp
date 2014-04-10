#include "ServerMaster.h"

ServerMaster::ServerMaster(int nPlayers, int nCardSets)
{
    playerCount = nPlayers;
    cardSetCount = nCardSets;
    remainingCards = new CardSet(cardSetCount);

    usedCards = new CardSet[playerCount];//need to initialize for each player in constructor
    bottomCards = new CardSet(4 * cardSetCount);

    levelGap = LEVEL_GAP_DEFAULT;
    playerScore = new int[playerCount];
    currentRound = 0;
    banker = PLAYER_NONE;
    firstPlayer = PLAYER_NONE;
    //randomize the card sets
    //create sockets for each player/worker
    //listen to each sockets
    //initailize workes

    MemSet(playingLevel, 0, 2);
    currentPrime = INVALID_CARD;
}

ServerMaster::~ServerMaster()
{
    //delete card sets
    delete remainingCards;
    delete usedCards;
    delete bottomCards;

    //kill workers
    //close sockets
}

void ServerMaster::Reset()//reset all data
{
    remainingCards.Clear();
    usedCards.Clear();
    bottomCards.Clear();

    for(int i = 0; i < playerCount; i++)
    {
        usedCards[i].Clear();
    }

    MemSet(playerScore, 0, playerCount);
    currentRound = 0;
    banker = PLAYER_NONE;
    firstPlayer = PLAYER_NONE;
        
    MemeSet(playingLevel, 0, 2);
    currentPrime = INVALID_CARD;
}

void ServerMaster::Shuffle()
{
    remainingCards.RandomizeCardSet(cardSetCount);
}

void ServerMaster::DispatchCard(Worker * workers)
{
    Card cardToDispatch;
    int curPlayer = firstPlayer;
    
    do
    {
        cardToDispatch = remainingCards.GetFirstCard();
        workers[curPlayer].FetchCard(cardToDispatch);
        curPlayer = ((curPlayer + 1) == MAX_PLAYER_COUNT) ? PLAYER_1 : (curPlayer + 1);
    }while(remainingCards.GetCount() > 8);
}

void ServerMaster::ExchangeCard(Worker bankerWorker)
{
    Card cardToDispatch;
    for (int idx = 0; idx < 8; idx++)
    {
        cardToDispatch = remainingCards.GetFirstCard();
        bankerWorker.FetchCard(cardToDispatch);
    }

    //TODO: wait for banker to return cards to bottomCards, maybe wait for a flag?
}

void ServerMaster::PlayOneRound()
{
    Card 
    for()
    {
        card = worker[idx].PlayOneCard();
    }
    
    // compare 4 cards
    // calculate score in this round
}

void ServerMaster::RecordScore()
{
    if ((PLAYER_1 == banker) || (PLAYER_3 == banker))
    {
        if (playerScore[1] >= 80)
        {
            //turn over
            banker++;
            //won't have below issue, need to move to else..
            if (MAX_PLAYER_COUNT == banker)
            {
                banker = PLAYER_1;
            }
        }
        else
        {
            if()
        }
    }
    else
    {
        ...
    }
}

void ServerMaster::GetScore(int* scoreContainer)
{
    for (int idx = PLAYER_1; idx < playerCount; idx++)
    {
        scoreContainer[idx] = playerScore[idx];
    }
}


int ServerMaster::GetCurrentRound()
{
    return currentRound;
}

int ServerMaster::GetPlayingLevel(PLAYERNAME player)
{
    if ((PLAYER_1 == player) || (PLAYER_3 == player))
    {
        return playingLevel[0];
    }
    else
    {
        return playingLevel[1];
    }
}

Card ServerMaster::GetCurrentPrime()
{
    return currentPrime;
}

int ServerMaster::GetCurrentRound()
{
    return currentRound;
}

int ServerMaster::GetBanker(void)
{
    return banker;
}

void ServerMaster::SetBanker(int newBanker)
{
    banker = newBanker;
}

void ServerMaster::SetLevelGap(int gap)
{
    levelGap = gap;
}

bool ServerMaster::IsBanker(int player)
{
    return (player == banker);
}
