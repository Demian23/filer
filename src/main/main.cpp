#include <iostream>
#include "../Filer.h"
#include "../IThrowable.h"

int main(int argc, char**argv)
{
    try{
        auto filer = Filer::makeInstanceFromArgs(argc, argv);
        if(filer->autorun()){
            filer->runTransformation();
        }else
            filer->run();
    }catch(IThrowable* e){
        std::cerr << e->errorDescription() << '\n';
        delete e;
        exit(1);
    }
    return 0;
}
