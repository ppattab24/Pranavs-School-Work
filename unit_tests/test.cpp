#include "../src/parse.cpp"
#include "../src/manager.cpp"
#include "../src/genuse.cpp"
#include "../src/shuntingYard.cpp"
#include "../src/token.cpp"
#include "gtest/gtest.h"

TEST(Token_Class, getStatusTest)
{
	Token* t = new Token("ls", 1);
        EXPECT_EQ(1, t->getStatus());	
}

TEST(Token_Class, toStringTest)
{
	        Token* t = new Token("ls", 1);
		EXPECT_EQ("ls", t->toString());
}

TEST(Token_Class, setStatusTest)
{
	        Token* t = new Token("ls", 1);
		t->setStatus(false);
		EXPECT_EQ(1, t->getStatus());
}

int main(int argc, char **argv) {
	  ::testing::InitGoogleTest(&argc, argv);
	    return RUN_ALL_TESTS();
}
