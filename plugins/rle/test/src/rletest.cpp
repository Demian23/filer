#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include "../../src/rle.h"

TEST(RleSuit, transformOrdinary)
{
    std::stringstream input{"zzzzggqs"}, output;
    Rle tr;
    ASSERT_NO_THROW(tr.transform(input, output));
    ASSERT_EQ("\x04z\x02g\x82qs", output.str());
    ASSERT_TRUE(output.str().size() == 7);
}
TEST(RleSuit, transformLowEdge)
{
    std::stringstream input{"z"}, output;
    Rle tr;
    ASSERT_NO_THROW(tr.transform(input, output));
    ASSERT_EQ("\x81z", output.str());
    ASSERT_TRUE(output.str().size() == 2);
}
TEST(RleSuit, transformHighEdge)
{
    std::stringstream input{"zzzzzzzz"}, output;
    Rle tr;
    ASSERT_NO_THROW(tr.transform(input, output));
    ASSERT_EQ("\x08z", output.str());
    ASSERT_TRUE(output.str().size() == 2);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
