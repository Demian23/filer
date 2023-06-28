#include <bitset>
#include <vector>
#include <climits>
#include <iostream>
#include "synlfsr.h"

class synlfsr{
public:
    synlfsr(const std::string& polinom, const std::string& seed);
    bool cipherBit(bool bit);
    uint8_t cipherByte(uint8_t byte);
private:
    std::bitset<128> reg;
    std::vector<int> xorbit;
    uint8_t polinomPower;
};

// polinom: 18 11 or 23 5 or 23 18
// seed for 5 10110
synlfsr::synlfsr(const std::string& polinom, const std::string& seed)
{
    //TODO: make checks for polinom and seed
    reg.reset();
    std::bitset<128> temp(seed);
    reg |= temp;

    int i = polinom.find_first_of(' '), j = 0;
    std::string number = polinom.substr(j, i);
    polinomPower = std::stoi(number) - 1;
    j = i + 1, i = polinom.find(j, ' ');
    for(; i != std::string::npos;j = i + 1, i = polinom.find(j , ' ')){
       xorbit.push_back(std::stoi(polinom.substr(j, i)) - 1);
    }
}

bool synlfsr::cipherBit(bool bit)
{
    bool result = reg[polinomPower];
    for(int bit_index : xorbit){
        result ^= reg[bit_index];
    }
    bit ^= result;
    reg <<= 1;
    reg[0] = bit;
    return bit;
}

uint8_t synlfsr::cipherByte(uint8_t byte)
{
    std::bitset<CHAR_BIT> result;
    for(int i = 0; i < sizeof(byte) * CHAR_BIT; i++, byte <<= 1){
        result[i] = cipherBit(byte & 0b0000'0001);
    }
    return static_cast<uint8_t>(result.to_ulong());
}

