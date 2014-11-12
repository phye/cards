AUTOMAKE_OPTIONS=subdir-objects

bin_PROGRAMS = Card_UT CardComp_UT CardSet_UT
Card_UT_SOURCES = Card_UT.cpp ../src/Card.cpp ../src/CardComp.cpp
Card_UT_CPPFLAGS = -I ../tractor -isystem ../thirdparty/gtest/include 
Card_UT_LDFLAGS = -L../thirdparty/gtest/ -lgtest -Wall -pthread

CardComp_UT_SOURCES = CardComp_UT.cpp ../src/Card.cpp ../src/CardComp.cpp
CardComp_UT_CPPFLAGS = -I ../tractor -isystem ../thirdparty/gtest/include 
CardComp_UT_LDFLAGS = -L../thirdparty/gtest/ -lgtest -Wall -pthread

CardSet_UT_SOURCES = CardComp_UT.cpp ../src/Card.cpp ../src/CardComp.cpp
CardSet_UT_CPPFLAGS = -I ../tractor -isystem ../thirdparty/gtest/include 
CardSet_UT_LDFLAGS = -L../thirdparty/gtest/ -lgtest -Wall -pthread
