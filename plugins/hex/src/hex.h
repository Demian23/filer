#ifndef __Hex_h__
#define __Hex_h__

#include "../../../src/IStreamTransformer.h"

class Hex : public IStreamTransformer{
public:
    const char* type()override;
    void transform(std::istream& input, std::ostream& output)override; 
    void retransform(std::istream& input, std::ostream& output)override; 
};

DEFINE_PLUGIN(Hex, hex)

#endif // !__Hex_h__
