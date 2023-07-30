#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include "../../src/IThrowable.h"
#include "../../src/Filer.h"

class FilerTest : public ::testing::Test{
protected:
    const char* Argv[5]{0, "PluginDirectory", 
    "../plugins/bin", "PluginsExtension", ".so"};
    std::unique_ptr<Filer> filer;
    void SetUp()override
    {
        ASSERT_NO_THROW(
            try{
                filer = Filer::makeInstanceFromArgs(5, Argv);
            } catch (IThrowable* e){
                std::cerr << e->errorDescription() << '\n';
                delete e;
            }
        );
        ASSERT_NE(filer, nullptr);
    }
};

TEST_F(FilerTest, setConfigParamsManually)
{
    ASSERT_THROW(filer->setConfigParam("Autorun", "Hi"), IThrowable*);
    ASSERT_NO_THROW(filer->setConfigParam("Autorun", "true"));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
