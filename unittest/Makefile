CXX=g++
GTEST_DIR=~/Workspace/gtest

USER_DIR=../src

CPP_FLAGS += -I ${USER_DIR} -isystem ${GTEST_DIR}/include -pthread 
CXX_FLAGS += -g -Wall -pthread

TESTS = Card_UT CardSet_UT CardComp_UT

all : $(TESTS)

clean:
	rm -f $(TESTS)
	rm -f *.o
	rm -f display.log

Card_UT: Card_UT.cpp libgtest.a Card.o
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) $^ -o $@
	
CardComp_UT: CardComp_UT.cpp libgtest.a Card.o CardComp.o
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) $^ -o $@

CardSet_UT: CardSet_UT.cpp libgtest.a CardSet.o Card.o CardComp.o
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) $^ -o $@

Card.o: $(USER_DIR)/Card.cpp
	$(CXX) $(CPP_FLAGS) -c $^ -o $@
	
CardComp.o: $(USER_DIR)/CardComp.cpp
	$(CXX) $(CPP_FLAGS) -c $^ -o $@

CardSet.o: $(USER_DIR)/CardSet.cpp
	$(CXX) $(CPP_FLAGS) -c $^ -o $@
