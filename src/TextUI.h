#ifndef __TextUI_h__
#define __TextUI_h__

#include "IUserInterface.h"
#include "Filer.h"

class TextUI final: public IUserInterface{
public:
    TextUI(Filer* filer) : filer_(filer){}
    void runUI()override;
private:
    Filer* filer_;

};
#endif
