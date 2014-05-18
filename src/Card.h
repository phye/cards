#ifndef _CARD_H
#define _CARD_H

#include <iostream>
#include <map>
#include <stdint.h>
#include "error.h"

using std::map;
using std::ostream;

enum card_suit_t {
    CARD_INVALID_SUIT =0,
    DIAMOND,
    CLUB,
    HEART,
    SPADE,
    BJOKER,     //black-white Joker
    CJOKER,      //colorful Joker :)
};

enum card_val_t {
    CARD_VAL_INVALID=0,
    CARD_VAL_TWO =1,
    CARD_VAL_THREE,
    CARD_VAL_FOUR,
    CARD_VAL_FIVE,
    CARD_VAL_SIX,
    CARD_VAL_SEVEN,
    CARD_VAL_EIGHT,
    CARD_VAL_NINE,
    CARD_VAL_TEN,
    CARD_VAL_JACK,
    CARD_VAL_QUEEN,
    CARD_VAL_KING,
    CARD_VAL_ACE,
    CARD_VAL_JOKER=14
};

enum card_order_t {
    NOT_PLAYED = 0,
    FIRST_PLAYED,
    SECOND_PLAYED,
    THIRD_PLAYED,
    FOURTH_PLAYED
};

class Card {
public:
    static const map<const card_suit_t, const char*> suit_str_map;
    static const map<const card_val_t, const char*> val_str_map;
    static map<const card_suit_t,const char*> create_ss_map(void);
    static map<const card_val_t, const char*> create_vs_map(void);

public:
    Card(card_suit_t suit=CARD_INVALID_SUIT, card_val_t value=CARD_VAL_INVALID);
    //First 4 bit for suit, last 4 bit for val
    explicit Card(const uint8_t pair);
    ~Card() {}

    const card_suit_t Get_suit() const { return card_suit; }
    const card_val_t  Get_val() const { return card_val; }
    const card_order_t Get_order() const { return card_order; }
    const uint8_t Get_char() const;

    void Set_order(card_order_t order) { card_order = order;}

    friend bool operator== (const Card& lhs, const Card& rhs);
    friend bool lt_by_suit(const Card& lhs, const Card& rhs);
    friend bool operator< (const Card& lhs, const Card& rhs);
    friend ostream& operator<< (ostream& os, const Card& rhs);

private:
    inline bool Is_valid(const card_suit_t st, const card_val_t val) const;

private:
    card_suit_t     card_suit;
    card_val_t      card_val;     //card value
    card_order_t    card_order;
};
#endif
