#include <gtest/gtest.h>
#include <sstream>
#include "../../../../src/IThrowable.h"
#include "../../src/synlfsr.h"

TEST(SynLfsrSuit, parsePolinom){
    SynLfsr testing;
    testing.parsePolinom("18 5 ");
    ASSERT_EQ(testing.power(), 17);
    EXPECT_EQ(testing.xors().size(), 1);
    ASSERT_EQ(testing.xors()[0], 4);
}

TEST(SynLfsrSuit, parseParams){
    SynLfsr testing;
    try{
        testing.setOptions("23 22 18 14 : 11100011100010010100100");
    } catch(IThrowable* e){
        std::cerr << e->errorDescription() << '\n';
        delete e;
    }
    ASSERT_EQ(testing.power(), 22);
    EXPECT_EQ(testing.xors().size(), 3);
    ASSERT_EQ(testing.xors()[2], 13);
    ASSERT_EQ(testing.seed(), "11100011100010010100100");
}

class SynLfsrTest : public ::testing::Test{
protected:
    SynLfsr tested;
    std::stringstream input;
    std::stringstream transformed;
    std::stringstream retransformed;
    void SetUp()override{
        tested.setOptions("4 3 : 1101");
        uint8_t testval = 12;
        input.write(reinterpret_cast<const char*>(&testval), 1);
        tested.transform(input, transformed);
        tested.retransform(transformed, retransformed);
    };
};

TEST_F(SynLfsrTest, cipher)
{
    try{
        ASSERT_EQ(*transformed.str().data(), '\xB2');
    } catch(IThrowable* e){
        std::cerr << e->errorDescription() << '\n';
        delete e;
    }
}

TEST_F(SynLfsrTest, decipher)
{
    try{
        ASSERT_EQ(*retransformed.str().data(), '\f');
    } catch(IThrowable* e){
        std::cerr << e->errorDescription() << '\n';
        delete e;
    }
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
