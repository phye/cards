#include "ServerMaster.h"

ServerMaster::ServerMaster(int nPlayers, int nCardSets)
{
    //randomize the card sets
    //create sockets for each player/worker
    //listen to each sockets
    //initailize workes
}

ServerMaster::~ServerMaster()
{
    //delete card sets
    //kill workers
    //close sockets
}

void ServerMaster::GetScore(int* scoreContainer)
{
}

void ServerMaster::Reset()//reset all data
{
}

void ServerMaster::Shuffle(CardSet cardset)
{
}

void ServerMaster::DispatchCard()
{
}

void ServerMaster::ExchangeCard()
{
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