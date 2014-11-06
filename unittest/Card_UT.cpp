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
    Card card(HEART, CARD_VAL_FOUR);
    EXPECT_EQ(HEART, card.Get_suit());
    EXPECT_EQ(CARD_VAL_FOUR, card.Get_val());
    cout << card << endl;
}

TEST(Card, ConstructorWithChar)
{
    EXPECT_THROW(Card cd0(0), runtime_error);
    EXPECT_THROW(Card cd1(0xfa), runtime_error);
    uint8_t char_val = (SPADE <<4) | 0x9;
    Card cd2(char_val);
    EXPECT_EQ(SPADE, cd2.Get_suit());
    EXPECT_EQ(CARD_VAL_TEN, cd2.Get_val());
    EXPECT_EQ(0x49, cd2.Get_char());
    cout << cd2 << endl;
}

TEST(Card, ConstructorWithIllegalInput)
{
    EXPECT_THROW(Card card(DIAMOND), runtime_error);
}

TEST(Card, CopyConstructor)
{
    Card card_a(SPADE,CARD_VAL_SEVEN);
    Card card_b(card_a);
    EXPECT_EQ(card_a, card_b);
    EXPECT_EQ(0x46, card_a.Get_char());
    cout << card_a << card_b << endl;
}

TEST(Card, Assignment)
{
    Card card_a(SPADE, CARD_VAL_NINE);
    Card card_b = card_a;
    EXPECT_EQ(card_a, card_b);
    cout << card_a << card_b << endl;
}

TEST(Card, LessThan)
{
    Card a(DIAMOND, CARD_VAL_NINE);
    Card b(DIAMOND, CARD_VAL_TEN);
    Card c = b;
    Card d (SPADE, CARD_VAL_FIVE);
    EXPECT_EQ(true, a < b);
    EXPECT_EQ(false, c < b);
    EXPECT_EQ(true, b < d);     //By default SPADE > HEART > CLUB > DIAMOND

    Card::Set_prime_val(CARD_VAL_NINE);
    Card::Set_prime_suit(DIAMOND);
    EXPECT_EQ(false, a < b);
    EXPECT_EQ(false, b < d);     

    card_pair_t char_val = (SPADE <<4) | CARD_VAL_EIGHT;
    Card::Set_prime(char_val);
    EXPECT_EQ(true, a < b);
    EXPECT_EQ(true, b < d);
}
