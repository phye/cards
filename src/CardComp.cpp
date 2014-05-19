#include "CardComp.h"
#include "Card.h"
#include <assert.h>
#include <stdexcept>

using std::runtime_error;

//Please also update this matrix if you changed the definition of card_suit_t
//suit_matrix_display[Primer_suit-1][current_card_suit-1]
//Use BJOKER to represent NT status
const char CardComp::suit_matrix_display[5][6] = {
    {4, 1, 2, 3, 5, 6},     //Diamond as prime
    {1, 4, 3, 2, 5, 6},     //Club as prime
    {2, 1, 4, 3, 5, 6},     //Heart as prime
    {1, 2, 3, 4, 5, 6},     //SPADE as prime
    {0, 1, 2, 3, 5, 6},     //NT as prime
};

const char CardComp::suit_matrix_normal[5][6] = {
    {4, 1, 1, 1, 5, 6},
    {1, 4, 1, 1, 5, 6},
    {1, 1, 4, 1, 5, 6},
    {1, 1, 1, 4, 5, 6},
    {1, 1, 1, 1, 5, 6},
};

bool CardComp::Is_prime(const Card& cd) const
{
    if( suit_matrix_normal[prime_suit-1][cd.Get_suit()-1] >= 4 )
        return true;
    else
        return ( cd.Get_val() == prime_val);
}

int CardComp::Prime_weight(const Card& cd) const
{
    card_val_t val = cd.Get_val();
    if( val == CARD_VAL_JOKER){
        switch (cd.Get_suit()){
            case CJOKER:
                return 5;
                break;
            case BJOKER:
                return 4;
                break;
            default:
                return -1; //Impossible
        }
    }else if(val == prime_val) {
        if( cd.Get_suit() == prime_suit )
            return 3;
        else
            return 2;
    } else 
        return 1;
}

bool CardComp::Less_prime(const Card& lhs, const Card& rhs) const
{
    const char* p_arr = suit_matrix_display[prime_suit-1];
    assert( (Prime_weight(lhs)>=0) && (Prime_weight(rhs) >= 0) );
    if( Prime_weight(lhs) < Prime_weight(rhs) )
        return true;
    else if( Prime_weight(lhs) == Prime_weight(rhs) )
    {
        if( !lhs.Get_order() && !rhs.Get_order() ) {
            if( Prime_weight(lhs) > 1 ) //value is prime
                return (p_arr[lhs.Get_suit()-1] < p_arr[rhs.Get_suit()-1]);
            else 
                return (lhs.Get_val() < rhs.Get_val());
        } else if( lhs.Get_order() && rhs.Get_order() ){
            if( Prime_weight(lhs) > 1 ) //value is prime
                return lhs.Get_order() > rhs.Get_order();
            else { //Normal prime card
                if (lhs.Get_val() < rhs.Get_val())
                    return true;
                else if (lhs.Get_val() == rhs.Get_val())
                    return lhs.Get_order() > rhs.Get_order();
                else 
                    return false;
            }
        } else if( !lhs.Get_order() && rhs.Get_order() )
            throw runtime_error("Invalid CardComp: lhs is not played");
        else
            throw runtime_error("Invalid CardComp: rhs is not played");
    } else
        return false;
}

bool CardComp::Less_nonprime(const Card& lhs, const Card& rhs) const
{
    //Card is for display, and not for comparison between players
    const char* p_arr = suit_matrix_display[prime_suit-1]; 
    if( !lhs.Get_order() && ! rhs.Get_order() ) {
        if( p_arr[lhs.Get_suit()-1] < p_arr[rhs.Get_suit()-1] ) 
            return true;
        else if( p_arr[lhs.Get_suit()-1] == p_arr[rhs.Get_suit()-1] )
            //lhs and rhs must be of the same suit
            return lhs.Get_val() < rhs.Get_val();
        else
            return false;
    } else if( lhs.Get_order() && rhs.Get_order() ) {
        //Card is for comparison
        if( lhs.Get_suit() != rhs.Get_suit() ){
            if (lhs.Get_order() == rhs.Get_order())
                return p_arr[lhs.Get_suit()-1] < p_arr[rhs.Get_suit()-1];
            else
                return lhs.Get_order() > rhs.Get_order();
        }
        else {
            if( lhs.Get_val() == rhs.Get_val() )
                return lhs.Get_order() > rhs.Get_order();
            else 
                return lhs.Get_val() < rhs.Get_val();
        }
    } else if( !lhs.Get_order() && rhs.Get_order() )
        throw runtime_error("Invalid CardComp: lhs is not played");
    else
        throw runtime_error("Invalid CardComp: rhs is not played");
}

bool CardComp::operator() (const Card& lhs, const Card& rhs) const
{
    if( Is_prime(lhs) && Is_prime(rhs) )
        return Less_prime(lhs, rhs);
    else if( !Is_prime(lhs) && !Is_prime(rhs) )
        return Less_nonprime(lhs, rhs);
    else if( !Is_prime(lhs) && Is_prime(rhs) )
        return true;
    else 
        return false;
}
