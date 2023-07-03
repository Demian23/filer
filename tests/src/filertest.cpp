#include <gtest/gtest.h>
#include <iostream>
#include "../../src/IFiler.h"

TEST(FilerPreambuleSuit, plgPathMethod){
    std::string path;
    ASSERT_NO_THROW(path = IFiler::plgPath("hex", "./bin/testdir"));
    std::clog << "Absolute path to plugin: " << path << std::endl; 
}

TEST(FilerPreambuleSuit, filerCreation){
    const char* temp[] = {0, "config", "./bin/testdir/config"};
    IFiler* filer = 0;
    try{
        filer = IFiler::makeInstanceFromArgs(3, const_cast<char**>(temp));
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
