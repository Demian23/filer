#include <bitset>
#include <string>
#include <vector>
#include <climits>
#include <iostream>
#include "../../../src/IThrowable.h"
#include "synlfsr.h"

class OrdinaryError : public IThrowable{
public:
    OrdinaryError(std::string msg) : msg_(msg){}
    const std::string& errorDescription()const override{return msg_;}
private:
    std::string msg_;
};

class synlfsr{
public:
    synlfsr(uint8_t power, const std::vector<uint8_t>& bits, const std::string& seed);
    bool cipherBit(bool bit);
    uint8_t cipherByte(uint8_t inbyte, bool cipher);
private:
    std::bitset<128> reg;
    std::vector<uint8_t> xorbits;
    uint8_t polinomPower;
};

// polinom: 18 11 or 23 5 or 23 18
// seed for 5 10110
synlfsr::synlfsr(uint8_t power, const std::vector<uint8_t>& bits, const std::string& seed) 
    : xorbits(bits), polinomPower(power)
{
    //TODO: make checks for polinom and seed
    std::bitset<128> temp(seed);
    std::swap(reg, temp);
}

bool synlfsr::cipherBit(bool bit)
{
    bool result = reg[polinomPower];
    for(uint8_t bit_index : xorbits){
        result ^= reg[bit_index];
    }
    bit ^= result;
    reg <<= 1;
    return bit;
}

// TODO: test efficiency
uint8_t synlfsr::cipherByte(uint8_t inbyte, bool cipher)
{
    std::bitset<CHAR_BIT> result;
    std::bitset<CHAR_BIT> inbitset = inbyte;
    for(unsigned i = 0; i < CHAR_BIT; i++){
        result[i] = cipherBit(inbitset[i]);
        if(cipher) reg[0] = result[i];
        else reg[0] = inbitset[i];
    }
    return static_cast<uint8_t>(result.to_ulong());
}

const char* SynLfsr::type(){return "synlfsr";}

SynLfsr* loadsynlfsr(){return new SynLfsr;}

void SynLfsr::transform(std::istream& input, std::ostream& output)
{
    char buffer[2] = {};
    synlfsr cipher(polinomPower_, xorbits_, seed_);
    while(input.read(buffer, 1)){
        *buffer = static_cast<char>(
            cipher.cipherByte(static_cast<uint8_t>(*buffer), true));
        output.write(buffer, 1);
    }
} 

void SynLfsr::retransform(std::istream& input, std::ostream& output)
{
    char buffer[2] = {};
    synlfsr cipher(polinomPower_, xorbits_, seed_);
    while(input.read(buffer, 1)){
        *buffer = static_cast<char>(
            cipher.cipherByte(static_cast<uint8_t>(*buffer), false));
        output.write(buffer, 1);
    }
}

void SynLfsr::parsePolinom(const std::string& polinom)
{
    char buffer[16] = {};
    int j = 0;
    for(char symbol : polinom){
        if(isdigit(symbol)){
            buffer[j++] = symbol;
        }else{
            if(symbol == ' ' && j > 0){
                uint8_t res = static_cast<uint8_t>(atoi(buffer));
                if(--res >= 0){
                    if(polinomPower_) xorbits_.push_back(res);
                    else polinomPower_ = res;
                    memset(buffer, 0, 16);
                    j = 0;
                }else 
                    throw new OrdinaryError("Can't valid cast" + std::string(buffer));
            }else
                throw new OrdinaryError(std::string("Wrong symbol:") + symbol); 
        }
    }
}

// options passed as polinom : seed
void SynLfsr::setOptions(const std::string& options)
{
    std::string::size_type delim_pos= options.find(':'); 
    if(delim_pos != std::string::npos){
        parsePolinom(options.substr(0, delim_pos));
        seed_ = options.substr(delim_pos + 2, options.size()-delim_pos); 
        if(seed_.size() >= polinomPower_){
            for(char symbol : seed_)
                if(symbol != '1' && symbol != '0')
                    throw new OrdinaryError(std::string("Wrong symbol:") + symbol);
        }else 
            throw new OrdinaryError("seed size < " + std::to_string(polinomPower_));
    } else 
        throw new OrdinaryError("must be -> polinom : seed");
}
