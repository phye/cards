#ifndef _CARD_H
#define _CARD_H

#include <iostream>
#include <map>
#include "error.h"

using std::map;
using std::ostream;

#define CARD_MIN_VAL 1
#define CARD_MAX_VAL 14     //14 for Joker

enum card_suit_t {
    CARD_INVALID_SUIT =0,
    DIAMOND,
    CLUB,
    HEART,
    SPADE,
    BJOKER,     //black-white Joker
    CJOKER,      //colorful Joker :)
};
typedef char card_val_t;
typedef bool card_state_t; 

class Card {
public:
    static const map<const card_suit_t, const char*> suit_str_map;
    static const map<const card_val_t, const char*> val_str_map;
    static map<const card_suit_t,const char*> create_ss_map(void);
    static map<const card_val_t, const char*> create_vs_map(void);

public:
    Card(card_suit_t suit=CARD_INVALID_SUIT, card_val_t value=0,card_state_t state=0 )
        : card_suit(suit), card_val(value), card_state(state) {}
    ~Card() {}

    const card_suit_t Get_suit() const { return card_suit; }
    const card_val_t  Get_val() const { return card_val; }
    const card_state_t Get_state() const { return card_state; }

    bool Is_good() const;

    //A card's suit and val should not be changed
    void Set_state(card_state_t state ) { card_state = state; }

    friend bool operator== (const Card& lhs, const Card& rhs);
    friend bool lt_by_suit(const Card& lhs, const Card& rhs);
    friend bool operator< (const Card& lhs, const Card& rhs);
    friend ostream& operator<< (ostream& os, const Card& rhs);

private:
    const card_suit_t     card_suit;
    const card_val_t      card_val;     //card value
    card_state_t    card_state;   // 1 for on, can be seen by all people
};
#endif
