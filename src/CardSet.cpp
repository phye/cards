#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <set>
#include <map>
#include "CardSet.h"
#include "Card.h"
#include "CardComp.h"

using std::multiset;
using std::ofstream;
using std::ios;
using std::endl;
using std::runtime_error;

CardSet::CardSet(int num, bool partial, const Card* pcd) 
    : num_of_card_set(num), is_partial(partial)
{
    if (pcd == NULL)
        card_set = new multiset<Card, CardComp> (CardComp(BJOKER, CARD_VAL_JOKER));
    else 
        card_set = new multiset<Card, CardComp> (CardComp(*pcd));

    if (!partial){
        for (int i=0; i< num_of_card_set; i++){
            char val;
            for(val=CARD_VAL_TWO; val<CARD_VAL_JOKER; val++)
            {
                Card sp(SPADE, (card_val_t) val);
                Add_card(sp);
                Card ht(HEART, (card_val_t) val);
                Add_card(ht);
                Card cl(CLUB, (card_val_t) val);
                Add_card(cl);
                Card dm(DIAMOND, (card_val_t) val);
                Add_card(dm);
                //Display();
            }
            Card bj(BJOKER, (card_val_t) val);
            Add_card(bj);
            Card cj(CJOKER, (card_val_t) val);
            Add_card(cj);
        }
    }
}

CardSet::CardSet(int num, const uint8_t* pair_arr, const size_t sz, const Card* pcd)
    : num_of_card_set(num), is_partial(true)
{
    if ( !pcd )
        card_set = new multiset<Card, CardComp> (CardComp(BJOKER, CARD_VAL_JOKER));
    else 
        card_set = new multiset<Card, CardComp> (CardComp(*pcd));
    for( const uint8_t* ptr = pair_arr; ptr != pair_arr+sz; ptr++){
        //Add exception handling here
        Card cd(*ptr);
        if( !Add_card(cd) )
            throw runtime_error("Invalid pair_arr, more cards than allowed");
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

void CardSet::Get_randomized_vector(vector<Card>& vec)
{
    vec.clear();
    vec.assign(card_set->begin(), card_set->end());
    std::random_shuffle(vec.begin(), vec.end());
}

int CardSet::Get_point()
{
    multiset<Card, CardComp>::const_iterator iter = card_set->begin();
    int ret = 0;
    while( iter!= card_set->end() ){
        if(iter->Get_val() == CARD_VAL_FIVE)
            ret += 5;
        else if (iter->Get_val() == CARD_VAL_TEN || iter->Get_val() == CARD_VAL_KING)
            ret += 10;
        ++iter;
    }
    return ret;
}
