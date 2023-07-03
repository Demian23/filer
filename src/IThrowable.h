#ifndef __IThrowable_h__
#define __IThrowable_h__

#include <string>

class IThrowable{
public:
    virtual const std::string& errorDescription()const = 0;
    virtual ~IThrowable(){}
};

#endif
