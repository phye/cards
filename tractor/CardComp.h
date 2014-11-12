#ifndef _CARDCOMP_H
#define _CARDCOMP_H

#include <tractor/Card.h>

class CardComp {
public:
    static const char suit_matrix_display[5][6];     //Use BJOKER as NT status
    static const char suit_matrix_normal[5][6];
public:
    CardComp(card_suit_t cs=BJOKER, card_val_t cv=CARD_VAL_JOKER)   //NT
        : prime_suit(cs), prime_val(cv) {}
    CardComp(const Card& prime)
        : prime_suit(prime.Get_suit()), prime_val(prime.Get_val()) {}
    ~CardComp() {}
    bool operator() (const Card& lhs, const Card& rhs) const;
    bool Is_prime(const Card& ) const;

public:
    card_suit_t Get_suit() const { return prime_suit; }
    card_val_t Get_val() const { return prime_val; }

private:
    inline int Prime_weight(const Card& cd) const;
    bool Less_prime(const Card& lhs, const Card& rhs) const; //return true if less
    bool Less_nonprime(const Card& lhs, const Card& rhs) const;

//private:
//    CardComp(const CardComp&);  //STL needs copy ctor for functor
//    CardComp& operator=(const CardComp &);

private:
    const card_suit_t prime_suit;
    const card_val_t prime_val;
};

#endif 
