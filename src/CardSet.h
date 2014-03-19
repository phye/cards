#ifndef _CARD_SET_H
#define _CARD_SET_H

#include <set>
#include "Card.h"

using std::multiset;

class CardSet {
public:
    typedef bool (* Less) (const Card& lhs, const Card& rhs);
public:
    CardSet(int num=1); 
    ~CardSet();

    bool Add_card(const Card &);
    bool Del_card(const Card &);
    bool Is_empty() const;
    int Size() const { return card_set->size(); }
    void Display() const;
    void Set_prime(Less lf);

private:
    CardSet(const CardSet& ); //Forbidden copy constructure
    CardSet& operator= (const CardSet &); //Forbidden assignment

private:
    multiset<Card, Less>* card_set;
    int num_of_card_set;
};

#endif

