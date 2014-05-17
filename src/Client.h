#ifndef _CLIENT_H
#define _CLIENT_H

class Client
{
public:
    Client();
    ~Client();

    bool ConnectServer();
    
private:
    void PlayCards();
    void ChangeCards();
    

private:
    int fd;//socket connection
    CardSet cardsInHand;
    CardSet cardByFirstPlayer;//card played by first player, client need to save this to do sanity check
    Card prime;
    vector<int> playerScore(2);
    vector<int> playingLevel(2);
};

#endif 
