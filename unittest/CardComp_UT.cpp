#include "Card.h"
#include "CardComp.h"
#include "gtest/gtest.h"
#include <iostream>

TEST(CardComp, DefaultConstructor)
{
    CardComp comp;
    EXPECT_EQ(BJOKER, comp.Get_suit());
    EXPECT_EQ(CARD_MAX_VAL, comp.Get_val());
    EXPECT_EQ(true, comp.Get_display());
}

TEST(CardComp, HEARTAsPrime)
{
    Card cjoker(CJOKER, CARD_MAX_VAL);
    Card bjoker(BJOKER, CARD_MAX_VAL);
    Card ht_5(HEART, 5);    
    Card sp_5(SPADE, 5);
    Card dm_5(DIAMOND, 5);
    Card cl_5(CLUB, 5);
    Card ht_6(HEART, 7);
    Card ht_3(HEART, 3);
    Card sp_9(SPADE, 9);
    Card sp_8(SPADE, 8);
    Card dm_9(DIAMOND, 9);
    Card dm_7(DIAMOND, 7);
    Card cl_13(CLUB, 13);
    Card cl_1(CLUB, 1);

    CardComp Less(HEART,5,false);
    EXPECT_EQ(true, Less(bjoker, cjoker));
    EXPECT_EQ(true, Less(ht_5, bjoker));
    EXPECT_EQ(true, Less(sp_5, ht_5));
    EXPECT_EQ(false, Less(dm_5, sp_5));
    EXPECT_EQ(false, Less(cl_5, dm_5));
    EXPECT_EQ(true, Less(ht_6, ht_5));
    EXPECT_EQ(true, Less(ht_3, ht_6));
    EXPECT_EQ(true, Less(sp_9, ht_3));
    EXPECT_EQ(true, Less(sp_8, sp_9));
    EXPECT_EQ(true, Less(dm_7, ht_3));
    EXPECT_EQ(true, Less(dm_7, dm_9));
    EXPECT_EQ(true, Less(cl_13, ht_3));
    EXPECT_EQ(true, Less(cl_1, cl_13));
    EXPECT_EQ(false, Less(dm_9, sp_8));
    EXPECT_EQ(false, Less(cl_13, dm_7));
    EXPECT_EQ(false, Less(cl_13, sp_8));

    CardComp Less2(HEART,5,true);
    EXPECT_EQ(true, Less2(bjoker, cjoker));
    EXPECT_EQ(true, Less2(ht_5, bjoker));
    EXPECT_EQ(true, Less2(sp_5, ht_5));
    EXPECT_EQ(true, Less2(dm_5, sp_5));
    EXPECT_EQ(true, Less2(cl_5, dm_5));
    EXPECT_EQ(true, Less2(ht_6, ht_5));
    EXPECT_EQ(true, Less2(ht_3, ht_6));
    EXPECT_EQ(true, Less2(sp_9, ht_3));
    EXPECT_EQ(true, Less2(sp_8, sp_9));
    EXPECT_EQ(true, Less2(dm_7, ht_3));
    EXPECT_EQ(true, Less2(dm_7, dm_9));
    EXPECT_EQ(true, Less2(cl_13, ht_3));
    EXPECT_EQ(true, Less2(cl_1, cl_13));
    EXPECT_EQ(true, Less2(dm_9, sp_8));
    EXPECT_EQ(true, Less2(cl_13, dm_7));
    EXPECT_EQ(true, Less2(cl_13, sp_8));
}
