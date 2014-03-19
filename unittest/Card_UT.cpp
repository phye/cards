#include "Card.h"
#include "gtest/gtest.h"
#include <iostream>
using std::cout;
using std::endl;

TEST(Card, DefaultConstructor)
{
    Card card;
    EXPECT_EQ(CARD_INVALID_SUIT, card.Get_suit() );
    EXPECT_EQ(0, card.Get_val() );
    EXPECT_EQ(0, card.Get_state());
    EXPECT_EQ(0, card.Is_good());
}

TEST(Card, Constructor)
{
    Card card(HEART, 3);
    EXPECT_EQ(HEART, card.Get_suit());
    EXPECT_EQ(3, card.Get_val());
    EXPECT_EQ(0, card.Get_state());
    EXPECT_EQ(1, card.Is_good());
    cout << card << endl;
}

TEST(Card, ConstructorWithIllegalInput)
{
    Card card(DIAMOND, -1);
    EXPECT_EQ(-1, card.Get_val());
    EXPECT_EQ(0, card.Is_good());
}

TEST(Card, CopyConstructor)
{
    Card card_a(SPADE, 8);
    Card card_b(card_a);
    EXPECT_EQ(card_a, card_b);
    cout << card_a << card_b << endl;
}

TEST(Card, Assignment)
{
    Card card_a(SPADE, 8);
    Card card_b = card_a;
    EXPECT_EQ(card_a, card_b);
    cout << card_a << card_b << endl;
}
