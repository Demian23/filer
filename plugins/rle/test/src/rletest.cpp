#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include "../../src/rle.h"

TEST(RleTransformSuit, Ordinary)
{
    std::stringstream input{"zzzzggqs"}, output;
    Rle tr;
    ASSERT_NO_THROW(tr.transform(input, output));
    ASSERT_EQ("\x04z\x02g\x82qs", output.str());
    ASSERT_TRUE(output.str().size() == 7);
}
TEST(RleTransformSuit, LowEdge)
{
    std::stringstream input{"z"}, output;
    Rle tr;
    ASSERT_NO_THROW(tr.transform(input, output));
    ASSERT_EQ("\x81z", output.str());
    ASSERT_TRUE(output.str().size() == 2);
}
TEST(RleTransformSuit, HighEdge)
{
    std::stringstream input{"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"
     "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"
     "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"
     "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"}, output;
    Rle tr;
    ASSERT_NO_THROW(tr.transform(input, output));
    ASSERT_EQ("\x7Fz\x51z", output.str());
    ASSERT_TRUE(output.str().size() == 4);
}

TEST(RleRetransformSuit, Ordinary)
{
    std::stringstream input{"\x04z\x02g\x82qs"}, output;
    Rle tr;
    ASSERT_NO_THROW(tr.retransform(input, output));
    ASSERT_EQ("zzzzggqs", output.str());
}

TEST(RleRetransformSuit, LowEdge)
{
    std::stringstream input{"\x81z"}, output;
    Rle tr;
    ASSERT_NO_THROW(tr.retransform(input, output));
    ASSERT_EQ("z", output.str());
}

TEST(RleRetransformSuit, HighEdge)
{
    std::stringstream input{"\x7Fz\x51z"}, output;
    Rle tr;
    ASSERT_NO_THROW(tr.retransform(input, output));
    ASSERT_EQ(output.str().size(), 208);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
