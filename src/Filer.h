#ifndef __Filer_h__
#define __Filer_h__

#include "IFiler.h"
#include "IUserInterface.h"

class Filer : public IFiler{
public:
    Filer(const __configuration& config);
    inline bool ready()override{return false;}
    void process()override;
private:
    IUserInterface* ui_;
};

#endif
