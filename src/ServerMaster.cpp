#include "ServerMaster.h"

ServerMaster::ServerMaster(int nPlayers = MAX_PLAYER_COUNT, int nCardSets = 2)
{
    playerCount = nPlayers;
    cardSetCount = nCardSets;
    levelGap = LEVEL_GAP_DEFAULT;
 
    Reset();
}

ServerMaster::~ServerMaster()
{
    //kill workers
    //close sockets
}

void ServerMaster::Reset()//reset all data
{
    allCards.clear();
    usedCards.clear();
    cardsInHand.clear();
    bottomCards.Clear();
    cardPlayedInThisRound.clear();

    playerScore = new vector<int>(2,0)
        
    currentSet = 0;
    currentRound = 0;
    banker = PLAYER_NONE;
    firstPlayer = PLAYER_1;
        
    playingLevel = new vector<int>(2, 2);
    currentPrime = CARD_INVALID_VAL;
    doubleClaim = FALSE;
    currentState = SHUFFLE_CARDS;
    workerReadyFlag = 0;
}

bool ServerMaster::Init()
{
    bool status = TRUE;
    cout << "How many users? (Default = %d)" << playerCount << endl;
    cin >> playerCount;
    
    cout << "How many set of cards? (Default = %d)" << cardSetCount << endl;
    cin >> cardSetCount;

    cout << "Level gap? (Default = %d)" << levelGap << endl;
    cin >> levelGap;
    // May need to init the size of card sets?
    
    WaitPlayerReady();
    currentState = SHUFFLE_CARDS;
    return status;
}

void ServerMaster::WaitPlayerReady()
{
    //TODO: create sockets.
    //TODO: Leave to Ye.
    fds = new int [playerCount]; 
    //listen to sockets and wait for bind? I don't know how to write this.
    for(int idx = 0; idx < playerCount; idx++)
    {
        fds[idx] = accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
        if(-1 != fds[idx])
        {
            //For now, we just kill the program itself on error, error handling will be developed later.
            exit -1;
        }
    }
}

void ServerMaster::Run()
{
    while(1)
    {
        switch(currentState)
        {
            case SHUFFLE_CARDS:
            {
                Shuffle();
                currentState = DISPATCHING_CARDS;					
                // Fall through
            }

            case DISPATCHING_CARDS:
            {
                DispatchCard();
                currentState = WAITING_PRIME;  
                // Fall through
            }

            case WAITING_PRIME:
            {
                WaitForPrime();
                currentState = CHANGE_CARD;
                //Fall through
            }

            case CHANGE_CARD:
            {
                ExchangeCard();
                currentState = PLAYING;
                firstPlayer = banker;
                //Fall through
            }

            case PLAYING:
            { 
                PlayOneRound();

                currentState = RECORD_SCORE;
            }

            case RECORD_SCORE:
            {
                //Do some record
                //Choose first player of next round
                RecordScore();
                if(!IsLastRound())
                {
                    currentState = PLAYING;
                    break;
                   
                }
                else
                {
                    currentState = SET_END;
                    // Fall through
                }
            }
        
            case SET_END:
            {
                SetEnd();
                if((LEVEL_END <= playingLevel[0]) || (LEVEL_END <= playingLevel[1]))
                {
                    currentState = GAME_END;
                }
                else
                {
                    currentState = SHUFFLE_CARDS;
                }
                break;
            }
                
            case GAME_END:
            {
                GameEnd();
                if(SHUFFLE_CARDS == currentState)
                {
                    // play again
                    break;
                }
                else
                {
                    return;
                }
            }
        }
    }
}


void ServerMaster::Shuffle()
{
    CardSet cardset(cardSetCount);
    
    cardset.RandomizeCardSet(&allCards);
}

void ServerMaster::DispatchCard(Worker * workers)
{
    Card cardToDispatch;
    int curPlayer = firstPlayer;
    
    do
    {
        cardToDispatch = allCards.GetFirstCard();
        workers[curPlayer].Dispatch_card(&cardToDispatch);
        cardsInHand[curPlayer].Add_card(&cardToDispatch);
        WaitWorkerSetReady(curPlayer);
        curPlayer = (curPlayer + 1) % playerCount;
    }while(allCards.GetCount() > 8);
}

void ServerMaster::WaitForPrime()
{//TODO: need to set banker based on if it is the first round, it should be also implemented in ClaimPrime()
    int curPlayer = firstPlayer;
    int idx;

    sleep(5);//wait for 5s for player to claim other prime
    //workers who claims prime should wakeup me!!!
    //Note: validation of claimed prime is performed in ClaimPrime() which will be called by worker.
    //      And it will decide whether keep sleep or wakeup
    //      But how long should it be for the second sleep?

    //still nobody claims
    if(CARD_INVALID_VAL == currentPrime)
    {
        currentPrime = bottomCards.GetBiggest();

        if (1 == currentSet)
        {
            // First set, just let player 1 be banker.
            banker = PLAYER_1;
        }
        
        //notify workers
        for(curPlayer = PLAYER_1; curPlayer < playerCount; curPlayer++)
        {
            workers[curPlayer].NotifyPrime(currentPrime);
            WaitWorkerSetReady(curPlayer);
        }
    }
    
    for(curPlayer = PLAYER_1; curPlayer < playerCount; curPlayer++)
    {
        workers[curPlayer].NotifyBanker(banker);
        WaitWorkerSetReady(curPlayer);
    }
    
}

void ServerMaster::ExchangeCard()
{
    assert(banker == PLAYER_NONE);

    workers[banker].Notify_card_swap();
    WaitWorkerSetReady(banker);
    workers[banker].Swap_card(bottomCards);

    cardsInHand[banker].Add_card(bottomCards);//TODO: need to overload this function to accept CardSet
    WaitWorkerSetReady(banker);//banker should first send card then set ready.
    cardsInHand[banker].Del_card(bottomCards);//TODO: need to overload this function, too
                                                //ToCheck: make sure the bottomCards is changed by SwapCard
}

void ServerMaster::PlayOneRound()
{
    PLAYERNAME curPlayer = firstPlayer;
    
    do
    {
        workers[curPlayer].Notify_card_send();
        WaitWorkerSetReady(curPlayer);

        curPlayer = (curPlayer + 1) % playerCount;
    }while(curPlayer != firstPlayer);
}

void ServerMaster::RecordScore()
{
    CardSet bigCard = cardPlayedInThisRound[firstPlayer];
    PLAYERNAME winner = firstPlayer;
    PLAYERNAME player = (firstPlayer + 1) % MAX_PLAYER_COUNT;
    int tempScore = cardPlayedInThisRound[firstPlayer].GetScore();
    
    do
    {
        if(bigCard < cardPlayedInThisRound[player])
        {
            bigCard = cardPlayedInThisRound[player];
            winner = player;
        }
        tempScore += cardPlayedInThisRound[player].GetScore();
        player = (player + 1) % MAX_PLAYER_COUNT;
    }while(player != firstPlayer);

    playerScore[winner % 2] += tempScore;

    for(player = PLAYER_1; player < playerCount; player++)
    {
        workers[player].Notify_round_result(winner, tempScore);//Only notify the score for this round
        WaitWorkerSetReady(player);
    }

    currentRound++;
    cardPlayedInThisRound[]//TODO: need to clear it
    firstPlayer = winner;
}

void ServerMaster::SetEnd()
{
    int idx;
    PLAYER_NAME setWinner;

    //TODO: first calculate scores in bottom card
    
    //calculate new level and set new banker
    if ((PLAYER_1 == banker) || (PLAYER_3 == banker))
    {
        if (playerScore[1] >= 80)
        {
            //turn over
            banker = (banker + 1) % MAX_PLAYER_COUNT;
            playingLevel[1] += (playerScore[1] - 80) / levelGap;
        }
        else
        {
            banker = (banker + 2) % MAX_PLAYER_COUNT;
            playingLevel[0] += (playerScore[0] - 120) / levelGap + 1;
        }
    }
    else
    {
        if (playerScore[0] >= 80)
        {
            //turn over
            banker = (banker + 1) % MAX_PLAYER_COUNT;
            playingLevel[0] += (playerScore[0] - 80) / levelGap;
        }
        else
        {
            banker = (banker + 2) % MAX_PLAYER_COUNT;
            playingLevel[1] += (playerScore[1] - 120) / levelGap + 1;
        }
    }

    // TODO: need to clear variables for the next round
    currentPrime = CARD_INVALID_VAL;
    doubleClaim = FALSE;
    playerScore[0] = playerScore[1] = 0;

    for(idx = PLAYER_1; idx < playerCount; idx++)
    {
        workers[idx].Notify_set_result(banker, playerScore);//The new banker is the winner of this set
        WaitWorkerSetReady(idx);
    }
}

void ServerMaster::GameEnd()
{
    //TODO: notify the worker, Show the game result, wait ready
    int idx;
    for(idx = PLAYER_1; idx < playerCount; idx++)
    {
        workers[idx].Notify_game_result(playerScore);
        WaitWorkerSetReady(idx);
    }
    
    //exit program or start another game by user's choice
    bool playAgain;
    cout << "Wanner play again? (Y/N)" << endl;
    cin >> playAgain;

    if (('y' == playAgain) || ('Y' == playAgain))
    {
        Reset();
        //currentState = SHUFFLE_CARDS;
    }
}

void ServerMaster::GetScore(int* scoreContainer)
{
    for (int idx = PLAYER_1; idx < playerCount; idx++)
    {
        scoreContainer[idx] = playerScore[idx];
    }
}


int ServerMaster::GetCurrentSet()
{
    return currentSet;
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

int ServerMaster::GetBanker(void)
{
    return banker;
}

PLAYSTATE ServerMaster::GetCurrentState()
{
    return currentState;
}

bool ServerMaster::ClaimPrime(CardSet claimingCard, int workerID)
{
    // This function is so ugly
    bool isDoubleClaim;

    if (claimingCard.Size() > 2)
    {
        return FALSE;
    }
    
    isDoubleClaim = (2 == claimingCard.Size()) ? TRUE : FALSE;
    
    if(CARD_INVALID_VAL == currentPrime)
    {
        currentPrime = claimingCard;
        doubleClaim = isDoubleClaim;
        if(1 == currentSet)
        {
            banker = workerID;
        }
        return TRUE;
    }
    else
    {
        if((BJOKER == claimingCard) || (CJOKER == claimingCard))
        {
            if((BJOKER != currentPrime) && (CJOKER != currentPrime))
            {
                currentPrime = claimingCard;
                doubleClaim = TRUE;
                if(1 == currentSet)
                {
                    banker = workerID;
                }
                return TRUE;
            }
        }
        else
        {
            if(isDoubleClaim && !doubleClaim)
            {
                currentPrime = claimingCard;
                doubleClaim = TRUE;
                if(1 == currentSet)
                {
                    banker = workerID;
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}

void ServerMaster::SetBanker(PLAYERNAME newBanker)
{
    banker = newBanker;
}


void ServerMaster::SetLevelGap(int gap)
{
    levelGap = gap;
}

void ServerMaster::SetNextReady(int workerId)
{
    workerReadyFlag |= (1 << workerId);
}

void ServerMaster::ReturnBottomCard(CardSet returnedCard)
{
    assert(0 == bottomCards.Size());
    bottomCards = returnedCard;
    cardsInHand[banker].Del_card(returnedCard);
}

// Note: The client should make sure that the played cardset obeys basic rules
bool ServerMaster::IsValidSend(int workerId, CardSet cards)
{
    if(!SanityCheck(cards))
    {
        return FALSE;
    }
    
    if(1 == cards.Size())
    {
        if(workerId == firstPlayer)
        {
            return TRUE;
        }
        else 
        {
            if(cards.Color() == cardPlayedInThisRound[firstPlayer].Color())
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }
    else
    {
        if(workerId == firstPlayer)
        {
            //ToDo: Fix this ShuaiPai validation
        }
    }
}

void ServerMaster::SendCard(int workerId, CardSet cards)
{
    usedCards[workerId].Add_card(cards);
    cardsInHand[workerId].Del_card(cards);
    cardPlayedInThisRound[workerId].Add_card(cards);
}

bool ServerMaster::IsBanker(PLAYERNAME player)
{
    return (player == banker);
}

void ServerMaster::WaitWorkerSetReady(int workerID)
{
    while(1 != (workerReadyFlag & (1 << workerID)))
    {
        sleep(1);
    }
    workerReadyFlag = 0;
}

void ServerMaster::WaitWorkerSetReady()
{
    while(((1 << playerCount) - 1) != workerReadyFlag)
    {
        sleep(1);
    }
    workerReadyFlag = 0;
}

bool ServerMaster::IsLastRound()
{
    return (cardsInHand[banker]->GetCardCount() == 0);
}

bool ServerMaster::SanityCheck(CardSet cards)
{
    //TODO: need to complete this
    return TRUE;
}