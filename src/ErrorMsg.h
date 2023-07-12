#ifndef __ErrorMsg_h__
#define __ErrorMsg_h__

#include "IThrowable.h"

class ErrorMsg : public IThrowable{
public:
    ErrorMsg(const std::string& msg) : msg_(msg){}
    const std::string& errorDescription()const override{return msg_;};
private:
    std::string msg_;
};

#endif
