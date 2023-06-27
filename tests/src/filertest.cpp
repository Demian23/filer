#include <gtest/gtest.h>
#include <iostream>
#include "../../src/Filer.h"

TEST(FilerPreambuleSuit, plgPathMethod){
    std::string path;
    ASSERT_NO_THROW(path = Filer::plgPath("hex", "./bin/testdir"));
    std::clog << "Absolute path to plugin: " << path << std::endl; 
}

TEST(FilerPreambuleSuit, filerCreation){
    const char* temp[] = {0, "config", "./bin/testdir/config"};
    Filer* filer = 0;
    try{
        filer = Filer::makeInstanceFromArgs(3, const_cast<char**>(temp));
    } catch (const char* msg){
        std::cerr << msg << std::endl;
    }
    EXPECT_TRUE(filer != 0);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
