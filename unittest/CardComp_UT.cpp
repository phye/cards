#include "Card.h"
#include "CardComp.h"
#include "gtest/gtest.h"
#include <iostream>

TEST(CardComp, DefaultConstructor)
{
    CardComp comp;
    EXPECT_EQ(BJOKER, comp.Get_suit());
    EXPECT_EQ(CARD_VAL_JOKER, comp.Get_val());
}

TEST(CardComp, HEARTAsPrime)
{
    Card cjoker(CJOKER, CARD_VAL_JOKER);
    Card bjoker(BJOKER, CARD_VAL_JOKER);
    Card ht_5(HEART, CARD_VAL_FIVE);    
    Card sp_5(SPADE, CARD_VAL_FIVE);
    Card dm_5(DIAMOND, CARD_VAL_FIVE);
    Card cl_5(CLUB, CARD_VAL_FIVE);
    Card ht_a(HEART, CARD_VAL_ACE);
    Card ht_3(HEART, CARD_VAL_THREE);
    Card sp_9(SPADE, CARD_VAL_NINE);
    Card sp_8(SPADE, CARD_VAL_EIGHT);
    Card dm_9(DIAMOND, CARD_VAL_NINE);
    Card dm_7(DIAMOND, CARD_VAL_SEVEN);
    Card cl_k(CLUB, CARD_VAL_KING);
    Card cl_2(CLUB, CARD_VAL_TWO);

    CardComp Less(HEART,CARD_VAL_FIVE);
    EXPECT_EQ(false, Less(bjoker, bjoker));
    EXPECT_EQ(false, Less(ht_5, ht_5));
    EXPECT_EQ(false, Less(sp_5, sp_5));
    EXPECT_EQ(false, Less(dm_5, dm_5));
    EXPECT_EQ(false, Less(cl_5, cl_5));

    /* Compare with joker */
    EXPECT_EQ(true, Less(bjoker, cjoker));
    EXPECT_EQ(false, Less(cjoker, bjoker));
    EXPECT_EQ(true, Less(ht_5, bjoker));
    EXPECT_EQ(false, Less(bjoker,ht_5));
    EXPECT_EQ(true, Less(sp_8, bjoker));
    EXPECT_EQ(false, Less(bjoker,sp_8));
    EXPECT_EQ(true, Less(dm_5, bjoker));
    EXPECT_EQ(false, Less(bjoker,dm_5));
    EXPECT_EQ(true, Less(cl_2, bjoker));
    EXPECT_EQ(false, Less(bjoker,cl_2));

    /* Compare within prime */
    EXPECT_EQ(true, Less(sp_5, ht_5));
    EXPECT_EQ(false, Less(ht_5, sp_5));
    EXPECT_EQ(true, Less(dm_5, sp_5));
    EXPECT_EQ(false, Less(sp_5, dm_5));
    EXPECT_EQ(true, Less(cl_5, dm_5));
    EXPECT_EQ(false, Less(dm_5, cl_5));

    EXPECT_EQ(true, Less(ht_a, ht_5));
    EXPECT_EQ(false, Less(ht_5, ht_a));
    EXPECT_EQ(true, Less(ht_3, ht_a));
    EXPECT_EQ(false, Less(ht_a, ht_3));

    EXPECT_EQ(true, Less(ht_a, cl_5));
    EXPECT_EQ(false, Less(cl_5, ht_a));
    EXPECT_EQ(true, Less(ht_a, dm_5));
    EXPECT_EQ(false, Less(dm_5, ht_a));

    /* Compare prime with non-prime */
    EXPECT_EQ(true, Less(sp_9, ht_3));
    EXPECT_EQ(false, Less(ht_3, sp_9));
    EXPECT_EQ(true, Less(cl_k, ht_3));
    EXPECT_EQ(false, Less(ht_3, cl_k));
    EXPECT_EQ(true, Less(dm_7, ht_3));
    EXPECT_EQ(false, Less(ht_3, dm_7));
    EXPECT_EQ(true, Less(sp_8, cl_5));
    EXPECT_EQ(false, Less(cl_5, sp_8));

    /* Compare without prime */
    EXPECT_EQ(true, Less(sp_8, sp_9));
    EXPECT_EQ(false, Less(sp_9, sp_8));
    EXPECT_EQ(true, Less(dm_7, dm_9));
    EXPECT_EQ(false, Less(dm_9, dm_7));
    EXPECT_EQ(true, Less(cl_2, cl_k));
    EXPECT_EQ(false, Less(cl_k, cl_2));   

    EXPECT_EQ(true, Less(dm_9, sp_8));
    EXPECT_EQ(false, Less(sp_8, dm_9));
    EXPECT_EQ(true, Less(cl_k, dm_7));
    EXPECT_EQ(false, Less(dm_7, cl_k));
    EXPECT_EQ(true, Less(cl_k, sp_8));
    EXPECT_EQ(false, Less(sp_8, cl_k));
}

TEST(CardComp, CLUBAsPrimeWithSetOrder)
{
    Card cjoker(CJOKER, CARD_VAL_JOKER);
    Card bjoker(BJOKER, CARD_VAL_JOKER);
    Card ht_5(HEART, CARD_VAL_FIVE);    
    Card sp_5(SPADE, CARD_VAL_FIVE);
    Card dm_5(DIAMOND, CARD_VAL_FIVE);
    Card cl_5(CLUB, CARD_VAL_FIVE);
    Card ht_a(HEART, CARD_VAL_ACE);
    Card ht_3(HEART, CARD_VAL_THREE);
    Card sp_9(SPADE, CARD_VAL_NINE);
    Card sp_8(SPADE, CARD_VAL_EIGHT);
    Card dm_9(DIAMOND, CARD_VAL_NINE);
    Card dm_7(DIAMOND, CARD_VAL_SEVEN);
    Card cl_k(CLUB, CARD_VAL_KING);
    Card cl_2(CLUB, CARD_VAL_TWO);
    Card ht_10(HEART, CARD_VAL_TEN);
    Card sp_10(SPADE, CARD_VAL_TEN);
    Card dm_10(DIAMOND, CARD_VAL_TEN);
    Card cl_10(CLUB, CARD_VAL_TEN);
    Card ht_7(HEART, CARD_VAL_SEVEN);

    Card ht_5_earlier(HEART, CARD_VAL_FIVE);    
    Card sp_5_later(SPADE, CARD_VAL_FIVE);
    Card dm_5_later(DIAMOND, CARD_VAL_FIVE);
    Card cl_5_later(CLUB, CARD_VAL_FIVE);

    bjoker.Set_order(FIRST_PLAYED);
    cl_5.Set_order(FIRST_PLAYED);
    sp_5.Set_order(FIRST_PLAYED);
    dm_9.Set_order(FIRST_PLAYED);
    sp_10.Set_order(FIRST_PLAYED);
    ht_7.Set_order(FIRST_PLAYED);

    cl_k.Set_order(SECOND_PLAYED);
    ht_3.Set_order(SECOND_PLAYED);
    dm_5.Set_order(SECOND_PLAYED);
    cl_5_later.Set_order(SECOND_PLAYED);
    ht_5_earlier.Set_order(SECOND_PLAYED);
    ht_10.Set_order(SECOND_PLAYED);
        
    cjoker.Set_order(THIRD_PLAYED);
    sp_9.Set_order(THIRD_PLAYED);
    dm_7.Set_order(THIRD_PLAYED);
    sp_5_later.Set_order(THIRD_PLAYED);
    dm_5_later.Set_order(THIRD_PLAYED);
    dm_10.Set_order(THIRD_PLAYED);

    cl_2.Set_order(FOURTH_PLAYED);
    sp_8.Set_order(FOURTH_PLAYED);
    ht_a.Set_order(FOURTH_PLAYED);
    ht_5.Set_order(FOURTH_PLAYED);
    cl_10.Set_order(FOURTH_PLAYED);

    CardComp Less(CLUB, CARD_VAL_TEN);

    /* Different cards */
    /* Non-prime, first played should be bigger */
    EXPECT_EQ(true, Less(sp_9, dm_9));
    EXPECT_EQ(false, Less(dm_9, sp_9));
    EXPECT_EQ(true, Less(ht_3, dm_9));
    EXPECT_EQ(false, Less(dm_9, ht_3));
    EXPECT_EQ(true, Less(ht_a, dm_9));
    EXPECT_EQ(false, Less(dm_9, ht_a));

    /* Same color compare */
    EXPECT_EQ(true, Less(sp_5, sp_9));
    EXPECT_EQ(false, Less(sp_9, sp_5));
    EXPECT_EQ(true, Less(ht_3, ht_a));
    EXPECT_EQ(false, Less(ht_a, ht_3));
    EXPECT_EQ(true, Less(cl_5, cl_k));
    EXPECT_EQ(false, Less(cl_k, cl_5));

    EXPECT_EQ(true, Less(cl_k, bjoker));
    EXPECT_EQ(false, Less(bjoker, cl_k));
    EXPECT_EQ(true, Less(bjoker, cjoker));
    EXPECT_EQ(false, Less(cjoker, bjoker));

    /* Same cards with different order */
    EXPECT_EQ(true, Less(ht_5, ht_5_earlier));
    EXPECT_EQ(false, Less(ht_5_earlier, ht_5));
    EXPECT_EQ(true, Less(sp_5_later, sp_5));
    EXPECT_EQ(false, Less(sp_5, sp_5_later));
    EXPECT_EQ(true, Less(dm_5_later, dm_5));
    EXPECT_EQ(false, Less(dm_5, dm_5_later));
    EXPECT_EQ(true, Less(cl_5_later, cl_5));
    EXPECT_EQ(false, Less(cl_5, cl_5_later));

    /* Compare about tens */
    EXPECT_EQ(true, Less(sp_10, cl_10));
    EXPECT_EQ(false, Less(cl_10, sp_10));
    EXPECT_EQ(true, Less(dm_10, ht_10));
    EXPECT_EQ(false, Less(ht_10, dm_10));
    EXPECT_EQ(true, Less(dm_10, sp_10));
    EXPECT_EQ(false, Less(sp_10, dm_10));

    /* Two non-primes, the first is bigger */
    EXPECT_EQ(true, Less(ht_a, dm_5));
    EXPECT_EQ(false, Less(dm_5, ht_a));
    EXPECT_EQ(true, Less(sp_9, dm_5));
    EXPECT_EQ(false, Less(dm_5, sp_9));

    /* Compare with display order for the same player */
    EXPECT_EQ(true, Less(cl_5, bjoker));
    EXPECT_EQ(false, Less(bjoker, cl_5));
    EXPECT_EQ(true, Less(dm_9, sp_5));
    EXPECT_EQ(false, Less(sp_5, dm_9));
    EXPECT_EQ(true, Less(sp_9, sp_10));
    EXPECT_EQ(false, Less(sp_10, sp_9));
    EXPECT_EQ(true, Less(cl_5, sp_10));
    EXPECT_EQ(false, Less(sp_10, cl_5));
    EXPECT_EQ(true, Less(sp_5, ht_7));
    EXPECT_EQ(false, Less(ht_7, sp_5));
} 

TEST(CardComp, NTAsPrime)
{
    Card cjoker(CJOKER, CARD_VAL_JOKER);
    Card bjoker(BJOKER, CARD_VAL_JOKER);
    Card ht_5(HEART, CARD_VAL_FIVE);    
    Card sp_5(SPADE, CARD_VAL_FIVE);
    Card dm_5(DIAMOND, CARD_VAL_FIVE);
    Card cl_5(CLUB, CARD_VAL_FIVE);
    Card ht_a(HEART, CARD_VAL_ACE);
    Card ht_3(HEART, CARD_VAL_THREE);
    Card sp_9(SPADE, CARD_VAL_NINE);
    Card sp_8(SPADE, CARD_VAL_EIGHT);
    Card dm_9(DIAMOND, CARD_VAL_NINE);
    Card dm_7(DIAMOND, CARD_VAL_SEVEN);
    Card cl_k(CLUB, CARD_VAL_KING);
    Card cl_2(CLUB, CARD_VAL_TWO);
    Card sp_6(SPADE, CARD_VAL_SIX);
    Card ht_6(HEART, CARD_VAL_SIX);
    Card cl_6(CLUB, CARD_VAL_SIX);
    Card dm_6(DIAMOND, CARD_VAL_SIX);

    CardComp Less(BJOKER,CARD_VAL_SIX);
    EXPECT_EQ(false, Less(bjoker, bjoker));

    /* Compare with joker */
    EXPECT_EQ(true, Less(bjoker, cjoker));
    EXPECT_EQ(false, Less(cjoker, bjoker));
    EXPECT_EQ(true, Less(ht_5, bjoker));
    EXPECT_EQ(false, Less(bjoker,ht_5));
    EXPECT_EQ(true, Less(sp_8, bjoker));
    EXPECT_EQ(false, Less(bjoker,sp_8));
    EXPECT_EQ(true, Less(cl_2, cjoker));
    EXPECT_EQ(false, Less(cjoker,cl_2));
    EXPECT_EQ(true, Less(dm_9, cjoker));
    EXPECT_EQ(false, Less(cjoker,dm_9));

    /* Compare with other prime (6) */
    EXPECT_EQ(true, Less(sp_6, cjoker));
    EXPECT_EQ(false, Less(cjoker, sp_6));
    EXPECT_EQ(true, Less(ht_6, bjoker));
    EXPECT_EQ(false, Less(bjoker,ht_6));    
    EXPECT_EQ(true, Less(sp_8, sp_6));
    EXPECT_EQ(false, Less(sp_6, sp_8));    
    EXPECT_EQ(true, Less(cl_2, ht_6));
    EXPECT_EQ(false, Less(ht_6,cl_2));

    /* Same color, different value */
    EXPECT_EQ(true, Less(ht_5, ht_a));
    EXPECT_EQ(false, Less(ht_a, ht_5));
    EXPECT_EQ(true, Less(ht_3, ht_a));
    EXPECT_EQ(false, Less(ht_a, ht_3));
    EXPECT_EQ(true, Less(sp_8, sp_9));
    EXPECT_EQ(false, Less(sp_9, sp_8));
    EXPECT_EQ(true, Less(dm_7, dm_9));
    EXPECT_EQ(false, Less(dm_9, dm_7));
    EXPECT_EQ(true, Less(cl_2, cl_k));
    EXPECT_EQ(false, Less(cl_k, cl_2));
    
    /* Different color, same value */
    EXPECT_EQ(true, Less(ht_5, sp_5));
    EXPECT_EQ(false, Less(sp_5, ht_5));
    EXPECT_EQ(true, Less(dm_5, sp_5));
    EXPECT_EQ(false, Less(sp_5, dm_5));
    EXPECT_EQ(true, Less(dm_5, cl_5));
    EXPECT_EQ(false, Less(cl_5, dm_5));
    EXPECT_EQ(true, Less(cl_5, ht_5));
    EXPECT_EQ(false, Less(ht_5, cl_5));
    
    /* Different color, different value */
    EXPECT_EQ(true, Less(ht_3, sp_9));
    EXPECT_EQ(false, Less(sp_9, ht_3));    
    EXPECT_EQ(true, Less(dm_7, ht_3));
    EXPECT_EQ(false, Less(ht_3, dm_7));    
    EXPECT_EQ(true, Less(cl_k, ht_3));
    EXPECT_EQ(false, Less(ht_3, cl_k));    
    EXPECT_EQ(true, Less(dm_9, sp_8));
    EXPECT_EQ(false, Less(sp_8, dm_9));
    EXPECT_EQ(true, Less(dm_7, cl_k));
    EXPECT_EQ(false, Less(cl_k, dm_7));
    EXPECT_EQ(true, Less(cl_k, sp_8));
    EXPECT_EQ(false, Less(sp_8, cl_k));
}


TEST(CardComp, NTAsPrimeWithSetOrder)
{
    Card cjoker(CJOKER, CARD_VAL_JOKER);
    Card bjoker(BJOKER, CARD_VAL_JOKER);
    Card ht_6(HEART, CARD_VAL_SIX);    
    Card sp_6(SPADE, CARD_VAL_SIX);
    Card dm_6(DIAMOND, CARD_VAL_SIX);
    Card cl_6(CLUB, CARD_VAL_SIX);
    Card ht_a(HEART, CARD_VAL_ACE);
    Card ht_3(HEART, CARD_VAL_THREE);
    Card sp_9(SPADE, CARD_VAL_NINE);
    Card sp_8(SPADE, CARD_VAL_EIGHT);
    Card dm_9(DIAMOND, CARD_VAL_NINE);
    Card dm_7(DIAMOND, CARD_VAL_SEVEN);
    Card cl_k(CLUB, CARD_VAL_KING);
    Card cl_2(CLUB, CARD_VAL_TWO);

    Card cl_6_later(CLUB, CARD_VAL_SIX);
    Card ht_3_later(HEART, CARD_VAL_THREE);
    Card dm_7_later(DIAMOND, CARD_VAL_SEVEN);
    Card sp_8_earlier(SPADE, CARD_VAL_EIGHT);

    bjoker.Set_order(FIRST_PLAYED);
    cl_6.Set_order(FIRST_PLAYED);
    sp_6.Set_order(FIRST_PLAYED);
    dm_9.Set_order(FIRST_PLAYED);

    cl_k.Set_order(SECOND_PLAYED);
    ht_3.Set_order(SECOND_PLAYED);
    dm_6.Set_order(SECOND_PLAYED);
    cl_6_later.Set_order(SECOND_PLAYED);

    cjoker.Set_order(THIRD_PLAYED);
    sp_9.Set_order(THIRD_PLAYED);
    dm_7.Set_order(THIRD_PLAYED);
    sp_8_earlier.Set_order(THIRD_PLAYED);

    cl_2.Set_order(FOURTH_PLAYED);
    sp_8.Set_order(FOURTH_PLAYED);
    ht_a.Set_order(FOURTH_PLAYED);
    ht_6.Set_order(FOURTH_PLAYED);
    ht_3_later.Set_order(FOURTH_PLAYED);
    dm_7_later.Set_order(FOURTH_PLAYED);

    CardComp Less(BJOKER, CARD_VAL_EIGHT);//TO Fix: although it is Joker as prime, we also have other eight cards as prime

    /* Different card */
    /* Non joker, first is bigger */
    EXPECT_EQ(true, Less(sp_9, dm_9));
    EXPECT_EQ(false, Less(dm_9, sp_9));
    EXPECT_EQ(true, Less(ht_3, dm_9));
    EXPECT_EQ(false, Less(dm_9, ht_3));
    EXPECT_EQ(true, Less(ht_a, dm_9));
    EXPECT_EQ(false, Less(dm_9, ht_a));

    /* Same color compare */
    EXPECT_EQ(true, Less(sp_6, sp_9));
    EXPECT_EQ(false, Less(sp_9, sp_6));
    EXPECT_EQ(true, Less(cl_6, cl_k));
    EXPECT_EQ(false, Less(cl_k, cl_6));

    /* Two non-primes, the first is bigger */
    EXPECT_EQ(true, Less(ht_a, dm_6));
    EXPECT_EQ(false, Less(dm_6, ht_a));
    EXPECT_EQ(true, Less(sp_9, dm_6));
    EXPECT_EQ(false, Less(dm_6, sp_9));
    EXPECT_EQ(true, Less(cl_k, dm_9));
    EXPECT_EQ(false, Less(dm_9, cl_k));   

    /* Prime is bigger */
    EXPECT_EQ(true, Less(cl_k, bjoker));
    EXPECT_EQ(false, Less(bjoker, cl_k));
    EXPECT_EQ(true, Less(bjoker, cjoker));
    EXPECT_EQ(false, Less(cjoker, bjoker));
    EXPECT_EQ(true, Less(sp_9, sp_8));
    EXPECT_EQ(false, Less(sp_8, sp_9));
    EXPECT_EQ(true, Less(cl_k, sp_8));
    EXPECT_EQ(false, Less(sp_8, cl_k));
    EXPECT_EQ(true, Less(dm_7, sp_8));
    EXPECT_EQ(false, Less(sp_8, dm_7));

    /* Same card */
    EXPECT_EQ(true, Less(cl_6_later, cl_6));
    EXPECT_EQ(false, Less(cl_6, cl_6_later));
    EXPECT_EQ(true, Less(ht_3_later, ht_3));
    EXPECT_EQ(false, Less(ht_3, ht_3_later));
    EXPECT_EQ(true, Less(dm_7_later, dm_7));
    EXPECT_EQ(false, Less(dm_7, dm_7_later));
    EXPECT_EQ(true, Less(sp_8, sp_8_earlier));
    EXPECT_EQ(false, Less(sp_8_earlier, sp_8));

    /* Compare with display order for the same player */
    EXPECT_EQ(true, Less(ht_a, sp_8));
    EXPECT_EQ(false, Less(sp_8, ht_a));
    EXPECT_EQ(true, Less(cl_2, ht_a));
    EXPECT_EQ(false, Less(ht_a, cl_2));
    EXPECT_EQ(true, Less(dm_7_later, cl_2));
    EXPECT_EQ(false, Less(cl_2, dm_7_later));

}

