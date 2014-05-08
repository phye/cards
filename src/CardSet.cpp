#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include "CardSet.h"
#include "Card.h"
#include "CardComp.h"

using std::multiset;
using std::ofstream;
using std::ios;
using std::endl;

extern bool lt_by_suit(const Card&, const Card&);

CardSet::CardSet(int num)
    : num_of_card_set(num)
{
    card_set = new multiset<Card, Less> (lt_by_suit);
}

CardSet::~CardSet()
{
    delete card_set;
}

bool CardSet::Add_card(const Card& card)
{
    if (card.Is_good())
        if (card_set->count(card) < num_of_card_set){
            card_set->insert(card);
            return true;
        }
    return false;
}

bool CardSet::Del_card(const Card& card)
{
    //We only want to remove one card, so don't use erase(key) directly
    multiset<Card, Less>::iterator iter = card_set->find(card);
    if(iter != card_set->end()) {
        card_set->erase(iter);
        return true;
    }
    return false;
}

bool CardSet::Is_empty() const
{
    return card_set->empty();
}

void CardSet::Display() const
{
    ofstream log_file;
    log_file.open("display.log", ios::app);
    multiset<Card>::const_iterator iter = card_set->begin();
    for (; iter!= card_set->end(); ++iter)
        if (iter->Is_good())
            log_file << *iter;
    log_file.close();
}

void CardSet::Set_prime(Less lf)
{
    multiset<Card, Less>* tmp 
        = new multiset<Card, Less> (lf);
    tmp->insert(card_set->begin(), card_set->end());
    delete card_set;
    card_set =tmp;
}
