#include <iostream>
#include "../IFiler.h"
#include "../TextUI.h"
#include "../IThrowable.h"

int main(int argc, char**argv)
{
    IFiler* filer = 0;
    try{
        filer = Filer::makeInstanceFromArgs(argc, argv);
    }catch(const char* msg){
        std::cerr << msg << "\n";
        delete filer;
        exit(1);
    }

    if(filer->ready()){
        try{
            filer->process();
        }catch(IThrowable* e){
            std::cerr << e->errorDescription() << '\n';
            delete e;
            exit(1);
        }
        std::cout << "Autorun was successful\n";
    } else {
    // work with user
        filer->process();
    }
    delete filer;
    return 0;
}
