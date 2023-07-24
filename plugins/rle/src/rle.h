#ifndef __Rle_h__
#define __Rle_h__

#include "../../../src/IStreamTransformer.h"

class Rle : public IStreamTransformer{
public:
    const char* type()override;
    void transform(std::istream& input, std::ostream& output)override; 
    void retransform(std::istream& input, std::ostream& output)override; 
    void setOptions(const std::string& options)override;
};

DEFINE_PLUGIN(Rle, rle)

#endif
