#ifndef _CARDCOMP_H
#define _CARDCOMP_H

#include "Card.h"

class CardComp {
public:
    static const char suit_matrix_display[5][6];     //Use BJOKER as NT status
    static const char suit_matrix_normal[5][6];
public:
    CardComp(card_suit_t cs=BJOKER, card_val_t cv=0, bool dp=0) 
        : prime_suit(cs), prime_val(cv), display(dp) {}
    ~CardComp() {}
    bool operator() (const Card& lhs, const Card& rhs);
    bool Is_prime(const Card& );

public:
    card_suit_t Get_suit() { return prime_suit; }
    card_val_t Get_val() { return prime_val; }
    bool Get_display() { return display; }

private:
    inline int Prime_weight(const Card& cd);
    bool Less_prime(const Card& lhs, const Card& rhs); //return true if less
    bool Less_nonprime(const Card& lhs, const Card& rhs);

private:
    CardComp(const CardComp&);
    CardComp& operator=(const CardComp &);

private:
    const card_suit_t prime_suit;
    const card_val_t prime_val;
    bool display;   //For display, we may add some special order based on suit
};

#endif 
