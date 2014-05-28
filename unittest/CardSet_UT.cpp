#include "CardSet.h"
#include "Card.h"
#include "gtest/gtest.h"
#include <stdexcept>
using std::runtime_error;
using std::cout;
using std::endl;

TEST(CardSet, DefaultConstructor)
{
    CardSet cs;
    EXPECT_EQ(true, cs.Is_empty());
    EXPECT_EQ(0, cs.Size());
}

TEST(CardSet, CtorFullCardSet)
{
    CardSet cs(2, false, NULL);
    EXPECT_EQ(54*2, cs.Size());
}

TEST(CardSet, CtorWithArray)
{
}

TEST(CardSet, Add_card_And_Del_card)
{
    CardSet cs(2);
    EXPECT_EQ(true, cs.Is_empty());
    EXPECT_EQ(0, cs.Size());

    Card a(DIAMOND, CARD_VAL_FOUR);
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

    EXPECT_THROW(Card b, runtime_error);     //Invalid card
    
    Card c(SPADE, CARD_VAL_NINE);
    EXPECT_EQ(true, cs.Add_card(c));
    EXPECT_EQ(false, cs.Is_empty());
    EXPECT_EQ(2, cs.Size());
}

TEST(CardSet, SetPrime)
{
    CardSet cs(2, false, NULL);
    EXPECT_EQ(54*2, cs.Size());
    cs.Display();

    Card ht(HEART,CARD_VAL_JACK);
    cs.Set_prime(ht);
    cs.Display();

    Card sp(SPADE,CARD_VAL_FIVE);
    cs.Set_prime(sp);
    cs.Display();

    Card dm(DIAMOND, CARD_VAL_ACE);
    cs.Set_prime(dm);
    cs.Display();

    Card cl(CLUB, CARD_VAL_TWO);
    cs.Set_prime(cl);
    cs.Display();

    Card nt(BJOKER, CARD_VAL_THREE);
    cs.Set_prime(nt);
    cs.Display();
}

TEST(CardSet, Get_randomized_vector)
{
    CardSet cs(2, false, NULL);
    EXPECT_EQ(54*2, cs.Size());
    vector<Card> vec;
    cs.Get_randomized_vector(vec);
    vector<Card>::const_iterator iter;
    for (int i=0; i<4; i++){
        vector<Card>::const_iterator st = vec.begin() + vec.size()/4*i;
        vector<Card>::const_iterator ed = st + vec.size()/4;
        cout<<"Cards of user " << i <<endl;
        for (iter = st; iter != ed; iter++)
            cout<<"\t" << *iter << endl;
    }
}

TEST(CardSet, Get_point)
{
    CardSet cs_all(2, false, NULL);
    EXPECT_EQ(54*2, cs_all.Size());
    EXPECT_EQ(200, cs_all.Get_point());

    CardSet cs(2);
    EXPECT_EQ(0, cs.Size());
    EXPECT_EQ(0, cs.Get_point());
    Card dm_5(DIAMOND, CARD_VAL_FIVE);
    Card sp_6(SPADE, CARD_VAL_SIX);
    Card ht_k(HEART, CARD_VAL_KING);
    Card ht_7(HEART, CARD_VAL_SEVEN);
    Card cl_10(CLUB, CARD_VAL_TEN);
    Card bjoker(BJOKER, CARD_VAL_JOKER);

    cs.Add_card(dm_5);
    EXPECT_EQ(5, cs.Get_point());
    cs.Add_card(sp_6);
    EXPECT_EQ(5, cs.Get_point());
    cs.Add_card(ht_k);
    EXPECT_EQ(15, cs.Get_point());
    cs.Add_card(ht_7);
    EXPECT_EQ(15, cs.Get_point());
    cs.Add_card(cl_10);
    EXPECT_EQ(25, cs.Get_point());
    cs.Add_card(bjoker);
    EXPECT_EQ(25, cs.Get_point());
    
    cs.Del_card(ht_7);
    EXPECT_EQ(25, cs.Get_point());
    cs.Del_card(dm_5);
    EXPECT_EQ(20, cs.Get_point());
    cs.Del_card(cl_10);
    EXPECT_EQ(10, cs.Get_point());
    cs.Del_card(sp_6);
    EXPECT_EQ(10, cs.Get_point());
    cs.Del_card(ht_k);
    EXPECT_EQ(0, cs.Get_point());
    cs.Del_card(bjoker);
    EXPECT_EQ(0, cs.Get_point());
}
