#include <iostream>
#include <map>
#include <stdexcept>
#include "Card.h"
using std::map;
using std::ostream;
using std::endl;
using std::runtime_error;

map<const card_suit_t, const char*> Card::create_ss_map(void)
{
    map<const card_suit_t, const char*> ss_map;
    ss_map[DIAMOND] = "<DM>";
    ss_map[CLUB] = "<CL>";
    ss_map[HEART] = "<HT>";
    ss_map[SPADE] = "<SP>";
    ss_map[BJOKER] = "<BJ>";
    ss_map[CJOKER] = "<CJ>";
    return ss_map;
}

map<const card_val_t, const char*> Card::create_vs_map(void)
{
    map<const card_val_t, const char*> vs_map;
    vs_map[CARD_VAL_TWO] = "2";
    vs_map[CARD_VAL_THREE] = "3";
    vs_map[CARD_VAL_FOUR] = "4";
    vs_map[CARD_VAL_FIVE] = "5";
    vs_map[CARD_VAL_SIX] = "6";
    vs_map[CARD_VAL_SEVEN] = "7";
    vs_map[CARD_VAL_EIGHT] = "8";
    vs_map[CARD_VAL_NINE] = "9";
    vs_map[CARD_VAL_TEN] = "10";
    vs_map[CARD_VAL_JOKER] = "J";
    vs_map[CARD_VAL_QUEEN] = "Q";
    vs_map[CARD_VAL_KING] = "K";
    vs_map[CARD_VAL_ACE] = "A";
    vs_map[CARD_VAL_JOKER] = "JOKER";
    return vs_map;
}

const map<const card_suit_t, const char*> Card::suit_str_map = Card::create_ss_map();
const map<const card_val_t, const char*> Card::val_str_map = Card::create_vs_map();

Card::Card(card_suit_t suit, card_val_t value)
{
    if (!Is_valid(suit, value))
        throw runtime_error("Invalid suit or value for a card");
    card_suit = suit;
    card_val = value;
    card_order = NOT_PLAYED;
}

Card::Card(const uint8_t pair)
{
    card_suit_t suit = (card_suit_t) (( pair & 0xF0) >> 4);
    card_val_t value = (card_val_t) (( pair & 0x0F));

    if (!Is_valid(suit, value))
        throw runtime_error("Invalid suit or value for a card");
    card_suit = suit;
    card_val = value;
    card_order = NOT_PLAYED;
}

const uint8_t Card::Get_char() const
{
    uint8_t ret = card_val & 0x0F;
    ret |= ((card_suit<<4) & 0xF0);
    return ret;
}

bool Card::Is_valid(const card_suit_t st, const card_val_t val) const
{
    if (val == CARD_VAL_JOKER)
        return (st == BJOKER || st == CJOKER);
    else if (val>= CARD_VAL_TWO && val < CARD_VAL_JOKER)
        return ( st>= DIAMOND && st< BJOKER);
    else 
        return false;
}

bool operator== (const Card& lhs, const Card& rhs)
{
    return ( lhs.card_suit == rhs.card_suit && lhs.card_val == rhs.card_val ) ;
}

bool lt_by_suit(const Card& lhs, const Card& rhs)
{
    if ( lhs.card_suit < rhs.card_suit)
        return true;
    if ( (lhs.card_suit == rhs.card_suit)  && (lhs.card_val < rhs.card_val) )
        return true;
    return false;
}

//Suit first, value second
bool operator< (const Card& lhs, const Card& rhs)
{
    return lt_by_suit(lhs,rhs);
}

//User is responsible for check the validability of card
ostream& operator<< (ostream& os, const Card& rhs)
{
    os << rhs.suit_str_map.find(rhs.card_suit)->second << ","
        << rhs.val_str_map.find(rhs.card_val)->second << "\t" ;
    return os;
}
