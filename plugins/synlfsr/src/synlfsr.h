#ifndef __SynLfsr_h__
#define __SynLfsr_h__

#include "../../../src/IStreamTransformer.h"
#include <string>
#include <vector>
class SynLfsr final: public IStreamTransformer{
public:
    SynLfsr(): polinomPower_(0){}
    const char* type()override;
    void transform(std::istream& input, std::ostream& output)override; 
    void retransform(std::istream& input, std::ostream& output)override; 
    void setOptions(const std::string& options)override;
    void parsePolinom(const std::string& polinom);
    const std::vector<uint8_t>& xors()const{return xorbits_;}
    const std::string& seed()const{return seed_;}
    int power()const{return polinomPower_;}
private:
    SynLfsr& operator=(const SynLfsr&);
    SynLfsr(const SynLfsr&);
    std::vector<uint8_t> xorbits_;
    std::string seed_;
    uint8_t polinomPower_;
};

DEFINE_PLUGIN(SynLfsr, synlfsr)

#endif
