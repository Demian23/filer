#include <iostream>
#include "../Filer.h"

int main(int argc, char**argv)
{
    Filer* filer = 0;
    try{
        filer = Filer::makeInstanceFromArgs(argc, argv);
    } catch(const char* msg){
        std::cerr << msg << "\n";
        delete filer;
        exit(1);
    }
    if(filer->ready()){
        filer->process();
        std::cout << "Autorun was successful\n";
    } else {
    // work with user
    }
    delete filer;
    return 0;
}
