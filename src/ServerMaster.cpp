#include "ServerMaster.h"

ServerMaster::ServerMaster(int nPlayers, int nCardSets)
{
    playerCount = nPlayers;
    cardSetCount = nCardSets;
    remainingCards = new CardSet(cardSetCount);

    usedCards = new CardSet[playerCount];//need to initialize for each player in constructor
    bottomCards = new CardSet(4 * cardSetCount);

    playerScore = new int[playerCount];
    banker = PLAYER_NONE;
    //randomize the card sets
    //create sockets for each player/worker
    //listen to each sockets
    //initailize workes
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

void ServerMaster::GetScore(int* scoreContainer)
{
    scoreContainer = playerScore;
}

void ServerMaster::Reset()//reset all data
{
    remainingCards.Clear();
    bottomCards.Clear();

    for(int i = 0; i < playerCount; i++)
    {
        usedCards[i].Clear();
    }

    MemSet(playerScore, 0, playerCount);
    banker = PLAYER_NONE;
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
    }while(remainingCards.GetCount() > (cardSetCount * 4));
}

void ServerMaster::ExchangeCard()
{
    worker[banker].FetchCard(cardToDispatch)
}

void ServerMaster::PlayOneRound()
{
}

void ServerMaster::RecordScore()
{
}

void ServerMaster::SetBanker(int newBanker)
{
    banker = newBanker;
}

int ServerMaster::GetBanker(void)
{
    return banker;
}

bool ServerMaster::IsBanker(int player)
{
    return (player == banker);
}