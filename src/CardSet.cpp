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

CardSet::CardSet(int num, bool partial, const Card* pcd, bool display)
    : num_of_card_set(num), is_partial(partial)
{
    if (pcd == NULL)
        card_set = new multiset<Card, CardComp> (CardComp(BJOKER, CARD_MAX_VAL, display));
    else 
        card_set = new multiset<Card, CardComp> (CardComp(*pcd, display));

    if (!partial){
        for (int i=0; i< num_of_card_set; i++){
            card_val_t val;
            for(val=CARD_MIN_VAL; val<CARD_MAX_VAL; val ++)
            {
                Card sp(SPADE, val);
                Add_card(sp);
                Card ht(HEART, val);
                Add_card(ht);
                Card cl(CLUB, val);
                Add_card(cl);
                Card dm(DIAMOND, val);
                Add_card(dm);
            }
            Card bj(BJOKER, val);
            Add_card(bj);
            Card cj(CJOKER, val);
            Add_card(cj);
        }
    }
}

CardSet::~CardSet()
{
    delete card_set;
}

bool CardSet::Add_card(const Card& card)
{
    if (card_set->count(card) < num_of_card_set){
        card_set->insert(card);
        return true;
    }
    return false;
}

bool CardSet::Del_card(const Card& card)
{
    //We only want to remove one card, so don't use erase(key) directly
    multiset<Card, CardComp>::iterator iter = card_set->find(card);
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
    multiset<Card, CardComp>::const_iterator iter = card_set->begin();
    for (; iter!= card_set->end(); ++iter)
        log_file << *iter << endl; 
    log_file.close();
}

void CardSet::Set_prime(const Card& pm)
{
    multiset<Card, CardComp>* tmp 
        = new multiset<Card, CardComp> (CardComp(pm));
    tmp->insert(card_set->begin(), card_set->end());
    delete card_set;
    card_set =tmp;
}
