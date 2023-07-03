#ifndef __IUserInterface_h__
#define __IUserInterface_h__

#include "IFiler.h"
class IUserInterface{
public:
    virtual void runUI() = 0;
    virtual ~IUserInterface(){}
};

#endif
