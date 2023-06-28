#ifndef __SynLfsr_h__
#define __SynLfsr_h__

#include "../../../src/IStreamTransformer.h"
class SynLfsr: public IStreamTransformer{
public:
    const char* type()override;
    void transform(std::istream& input, std::ostream& output)override; 
    void retransform(std::istream& input, std::ostream& output)override; 
    void setOptions(const std::string& options)override;
};

DEFINE_PLUGIN(SynLfsr, synlfsr)

#endif
