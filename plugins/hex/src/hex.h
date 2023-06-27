#ifndef __Hex_h__
#define __Hex_h__

#include "../../../src/IStreamTransformer.h"

class Hex final: public IStreamTransformer{
public:
    Hex() : spaces_(false){}
    const char* type()override;
    void transform(std::istream& input, std::ostream& output)override; 
    void retransform(std::istream& input, std::ostream& output)override; 
    void setOptions(const std::string& options)override;
private:
    bool spaces_;
};

DEFINE_PLUGIN(Hex, hex)

#endif // !__Hex_h__
