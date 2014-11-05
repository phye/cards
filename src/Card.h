#ifndef _CARD_H
#define _CARD_H

#include <iostream>
#include <map>
#include <stdint.h>
#include "error.h"
#include "common.h"

using std::map;
using std::ostream;

class Card {
public:
    static const map<const card_suit_t, const char*> suit_str_map;
    static const map<const card_val_t, const char*> val_str_map;
    static map<const card_suit_t,const char*> create_ss_map(void);
    static map<const card_val_t, const char*> create_vs_map(void);

    static inline void Set_prime_suit(const card_suit_t ps=BJOKER) { prime_suit = ps; }
    static inline void Set_prime_val(const card_val_t pv=CARD_VAL_JOKER) { prime_val = pv; }
    static inline void Set_prime(const card_pair_t pair);
    static inline card_suit_t Get_prime_suit(void) { return prime_suit; }
    static inline card_val_t Get_prime_val(void) { return prime_val; }
    static inline card_pair_t Get_prime(void) 
    { 
        return ((prime_val & 0x0F) | ((prime_suit & 0x0F) << 4));
    }

public:
    Card(card_suit_t suit=CARD_INVALID_SUIT, card_val_t value=CARD_VAL_INVALID);
    //First 4 bit for suit, last 4 bit for val
    explicit Card(const card_pair_t pair);
    ~Card() {}

    const card_suit_t Get_suit() const { return card_suit; }
    const card_val_t  Get_val() const { return card_val; }
    const card_order_t Get_order() const { return card_order; }
    const card_pair_t Get_char() const;

    void Set_order(card_order_t order) { card_order = order;}

    friend bool operator== (const Card& lhs, const Card& rhs);
    friend bool operator< (const Card& lhs, const Card& rhs);
    friend ostream& operator<< (ostream& os, const Card& rhs);

private:
    inline bool Is_valid(const card_suit_t st, const card_val_t val) const;

private:
    card_suit_t     card_suit;
    card_val_t      card_val;     //card value
    card_order_t    card_order;
    static card_suit_t prime_suit;
    static card_val_t prime_val;
};
#endif
