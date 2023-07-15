#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include "../../src/haffman.h"

TEST(HaffmanAuxSuit, FrequencyTest)
{
    hfm::HaffmanCompression h;
    hfm::HaffmanHeap actual;
    std::stringstream input("JJJJJJJJfffffff111122b");
    std::vector<std::pair<char, int>> expected
    {{'b', 1},{'2', 2},{'1', 4},{'f', 7},{'J', 8}};
    std::reverse(expected.begin(), expected.end());
    auto compare = [](const std::pair<char, int>& f, 
                    const std::unique_ptr<hfm::Node>& s) -> bool
                    {return f.first == s->byte && f.second == s->frequency;};

    h.countFrequency(input, actual);
    for(int i = 0; i < 5; i++){ 
        auto temp = actual.extractMin();
        ASSERT_TRUE(compare(expected.back(), temp));
        expected.pop_back();
    }
}

TEST(HaffmanAuxSuit, SubsitutionTest)
{
    std::stringstream input("JJJJJJJJJffffffff111122b");
    hfm::HaffmanCompression h;
    hfm::HaffmanHeap actual;
    h.countFrequency(input, actual);
    h.fillConvertTable(actual.makeTree());
    ASSERT_EQ(h.substitution('J'), "0");
    ASSERT_EQ(h.substitution('f'), "11");
    ASSERT_EQ(h.substitution('1'), "101");
    ASSERT_EQ(h.substitution('2'), "1001");
    ASSERT_EQ(h.substitution('b'), "1000");
}

TEST(HaffmanSuit, TransformTest)
{
    std::stringstream input("JJJJJJJJJffffffff111122b");
    std::ofstream output("TransformTest");
    hfm::HaffmanCompression h;
    h.transform(input, output);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
