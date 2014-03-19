#include "CardSet.h"
#include "Card.h"
#include "gtest/gtest.h"

TEST(CardSet, DefaultConstructor)
{
    CardSet cs;
    EXPECT_EQ(true, cs.Is_empty());
    EXPECT_EQ(0, cs.Size());
}

TEST(CardSet, Add_card_And_Del_card)
{
    CardSet cs(2);
    EXPECT_EQ(true, cs.Is_empty());
    EXPECT_EQ(0, cs.Size());

    Card a(DIAMOND, 3);
    EXPECT_EQ(true,cs.Add_card(a));
    EXPECT_EQ(false, cs.Is_empty());
    EXPECT_EQ(1, cs.Size());

    EXPECT_EQ(true, cs.Add_card(a));
    EXPECT_EQ(false, cs.Is_empty());
    EXPECT_EQ(2, cs.Size());

    EXPECT_EQ(false, cs.Add_card(a));
    EXPECT_EQ(false, cs.Is_empty());
    EXPECT_EQ(2, cs.Size());

    EXPECT_EQ(true, cs.Del_card(a));
    EXPECT_EQ(false, cs.Is_empty());
    EXPECT_EQ(1, cs.Size());

    EXPECT_EQ(true, cs.Del_card(a));
    EXPECT_EQ(true, cs.Is_empty());
    EXPECT_EQ(0, cs.Size());

    EXPECT_EQ(false, cs.Del_card(a));
    EXPECT_EQ(true, cs.Is_empty());
    EXPECT_EQ(0, cs.Size());

    EXPECT_EQ(true,cs.Add_card(a));
    EXPECT_EQ(false, cs.Is_empty());
    EXPECT_EQ(1, cs.Size());

    Card b;     //Invalid card
    EXPECT_EQ(false, cs.Add_card(b));
    EXPECT_EQ(false, cs.Is_empty());
    EXPECT_EQ(1, cs.Size());
    
    Card c(SPADE, 10);
    EXPECT_EQ(true, cs.Add_card(c));
    EXPECT_EQ(false, cs.Is_empty());
    EXPECT_EQ(2, cs.Size());

    cs.Display();
}

TEST(CardSet, Set_prime)
{

}
