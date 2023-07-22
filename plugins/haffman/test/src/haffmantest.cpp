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
    std::stringstream output;
    hfm::HaffmanCompression h;
    ASSERT_NO_THROW(h.transform(input, output));
    ASSERT_EQ(output.str()[24], '\0');
    ASSERT_EQ(output.str()[25], '\xFE');
}

TEST(HaffmanSuit, TableWriting)
{
    std::stringstream input("JJJJJJJJJffffffff111122b");
    std::stringstream output;
    hfm::HaffmanCompression h;
    hfm::HaffmanHeap heap;
    h.countFrequency(input, heap);
    h.fillConvertTable(heap.makeTree());
    h.writeTable(output);
    ASSERT_EQ("f11\n21001\nb1000\n1101\nJ0\n", output.str());
}

TEST(HaffmanSuit, RestoringTable)
{
    std::stringstream input("f11\n21001\nb1000\n1101\nJ0\nLastString\n");
    hfm::HaffmanCompression h;
    h.restoreTable(input);
    ASSERT_EQ(h.substitution('J'), "0");
    ASSERT_EQ(h.substitution('f'), "11");
    ASSERT_EQ(h.substitution('1'), "101");
    ASSERT_EQ(h.substitution('2'), "1001");
    ASSERT_EQ(h.substitution('b'), "1000");
    std::string temp;
    std::getline(input, temp);
    ASSERT_EQ(temp, "LastString");
}

TEST(HaffmanSuit, Retransform)
{
    std::stringstream input("JJJJJJJJJffffffff111122b");
    std::stringstream output, retransformed;
    hfm::HaffmanCompression h;
    ASSERT_NO_THROW(h.transform(input, output));
    ASSERT_NO_THROW(h.retransform(output, retransformed));
    ASSERT_EQ(retransformed.str(), input.str());
}

TEST(HaffmanSuit, RetransformEdgeCase)
{
    std::stringstream input("Jf"), output, reoutput;
    hfm::HaffmanCompression h;
    ASSERT_NO_THROW(h.transform(input, output));
    ASSERT_NO_THROW(h.retransform(output, reoutput));
    ASSERT_EQ(reoutput.str(), input.str());
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
