#include "Card.h"
#include "gtest/gtest.h"
#include <iostream>
#include <stdexcept>
using std::cout;
using std::endl;
using std::hex;
using std::runtime_error;

TEST(Card, DefaultConstructor)
{
    EXPECT_THROW(Card card, runtime_error);
}

TEST(Card, Constructor)
{
    Card card(HEART, 3);
    EXPECT_EQ(HEART, card.Get_suit());
    EXPECT_EQ(3, card.Get_val());
    cout << card << endl;
}

TEST(Card, ConstructorWithChar)
{
    EXPECT_THROW(Card cd0(0), runtime_error);
    EXPECT_THROW(Card cd1(0xfa), runtime_error);
    uint8_t char_val = (SPADE <<4) | 0x9;
    Card cd2(char_val);
    EXPECT_EQ(SPADE, cd2.Get_suit());
    EXPECT_EQ(9, cd2.Get_val());
    cout << cd2 << endl;
}

TEST(Card, ConstructorWithIllegalInput)
{
    EXPECT_THROW(Card card(DIAMOND,-1), runtime_error);
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
