#include "Card.h"
#include <iostream>
#include <map>
using std::map;
using std::ostream;
using std::endl;

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
    vs_map[1] = "2";
    vs_map[2] = "3";
    vs_map[3] = "4";
    vs_map[4] = "5";
    vs_map[5] = "6";
    vs_map[6] = "7";
    vs_map[7] = "8";
    vs_map[8] = "9";
    vs_map[9] = "10";
    vs_map[10] = "J";
    vs_map[11] = "Q";
    vs_map[12] = "K";
    vs_map[13] = "A";
    vs_map[14] = "JOKER";
    return vs_map;
}

const map<const card_suit_t, const char*> Card::suit_str_map = Card::create_ss_map();
const map<const card_val_t, const char*> Card::val_str_map = Card::create_vs_map();

bool Card::Is_good() const
{
    if(card_val == CARD_MAX_VAL)
        if(card_suit == BJOKER || card_suit == CJOKER)
            return true;
        else
            return false;
    else
        return !( card_suit == CARD_INVALID_SUIT || card_val < CARD_MIN_VAL || card_val > CARD_MAX_VAL-1 );
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
    if ( rhs.Get_state() )
        os << rhs.suit_str_map.find(rhs.card_suit)->second << ","
            << rhs.val_str_map.find(rhs.card_val)->second << "\t" ;
    else
        os << "##" << "," << "**" << "\t";
    return os;
}
