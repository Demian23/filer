#ifndef __Base64_h__
#define __Base64_h__

#include "../../../src/IStreamTransformer.h"

class Base64 : public IStreamTransformer{
public:
    const char* type()override;
    void transform(std::istream& input, std::ostream& output)override; 
    void retransform(std::istream& input, std::ostream& output)override; 
    void setOptions(const std::string& options)override;
};

DEFINE_PLUGIN(Base64, base64)

#endif // !__Base64_h__
