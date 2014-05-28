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

TEST(CardSet, FullCardSet)
{
    CardSet cs(2, false, NULL);
    EXPECT_EQ(54*2, cs.Size());
    cs.Display();
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

    //cs.Display();
}

