#ifndef CARD_COMMON_H
#define CARD_COMMON_H

#include <stdint.h>

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

typedef uint8_t card_pair_t;

#endif
