#ifndef __AutoFiler_h__
#define __AutoFiler_h__

#include "IFiler.h"
#include "IStreamTransformer.h"
#include <fstream>

class AutoFiler: public IFiler{
public:
    void process()override;
    bool ready()override{return true;}
    AutoFiler(const __configuration& config);
    virtual ~AutoFiler();
private:
    std::ifstream in;
    std::ofstream out;
    std::string options;
    void* plg_handle;
    IStreamTransformer* transformation;
    bool transform;
    AutoFiler(const AutoFiler&);     
    void getTransformation(const std::string& plg_way);
};

#endif
