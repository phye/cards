#ifndef _CARD_SET_H
#define _CARD_SET_H

#include <set>
#include <vector>
#include "Card.h"
#include "CardComp.h"

using std::multiset;
using std::vector;

class CardSet {
public:
public:
    CardSet(int num=2, bool partial=true, const Card* pcd=NULL); 
    CardSet(int num, const uint8_t* pair_arr, const size_t sz, const Card* pcd=NULL);
    ~CardSet();

    bool Add_card(const Card &);
    bool Del_card(const Card &);
    bool Is_empty() const;
    int Size() const { return card_set->size(); }
    void Display() const;
    void Set_prime(const Card &);

    void Get_randomized_vector(vector<Card>& vec);
    int Get_point();
private:
    CardSet(const CardSet& ); //Forbidden copy constructure
    CardSet& operator= (const CardSet &); //Forbidden assignment

private:
    multiset<Card, CardComp>* card_set;
    int num_of_card_set;
    bool is_partial;
};

#endif
