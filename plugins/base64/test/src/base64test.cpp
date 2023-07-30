#include <gtest/gtest.h>
#include <iostream>
#include "../../src/base64.h"

TEST(Base64Suit, transform3Bytes){
    Base64 coder; 
    std::stringstream input("Man"), output;
    coder.transform(input, output);
    ASSERT_EQ(output.str(), "TWFu");
}
TEST(Base64Suit, transform2Bytes){
    Base64 coder; 
    std::stringstream input("Ma"), output;
    coder.transform(input, output);
    ASSERT_EQ(output.str(), "TWE=");
}
TEST(Base64Suit, transform1Bytes){
    Base64 coder; 
    std::stringstream input("M"), output;
    coder.transform(input, output);
    ASSERT_EQ(output.str(), "TQ==");
}

TEST(Base64Suit, decode3Byte)
{
   Base64 coder; 
   std::stringstream decode("TWFu");
   std::stringstream result;
   coder.retransform(decode, result);
   ASSERT_EQ(result.str(), "Man");
}
TEST(Base64Suit, decode2Byte)
{
   Base64 coder; 
   std::stringstream decode("TWE=");
   std::stringstream result;
   coder.retransform(decode, result);
   ASSERT_EQ(result.str().substr(0, 2), "Ma");
}
TEST(Base64Suit, decode1Byte)
{
   Base64 coder; 
   std::stringstream decode("TQ==");
   std::stringstream result;
   coder.retransform(decode, result);
   ASSERT_EQ(result.str().substr(0, 1), "M");
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
