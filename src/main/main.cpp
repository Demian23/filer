#include <iostream>
#include "../Filer.h"
#include "../IThrowable.h"

int main(int argc, char**argv)
{
    Filer* filer = 0;
    try{
        filer = Filer::makeInstanceFromArgs(argc, argv);
        if(filer->autorun()){
            filer->runTransformation();
        }else
            filer->run();
    }catch(IThrowable* e){
        std::cerr << e->errorDescription() << '\n';
        delete e;
        delete filer;
        exit(1);
    }
    delete filer;
    return 0;
}
