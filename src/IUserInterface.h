#ifndef __IUserInterface_h__
#define __IUserInterface_h__

class IUserInterface{
public:
    virtual void runUI() = 0;
    virtual const char* type() = 0;
    virtual ~IUserInterface(){}
};

#endif
