#include "hex.h"
#include <cstdio>
#include <iostream>

Hex* loadhex(){return new Hex;}
const char* Hex::type()
{
    return "hex";
}
void Hex::transform(std::istream &input, std::ostream &output)
{
    char byte = 0;
    char buffout[3] = {};
    while(input.read(&byte, 1)){
        snprintf(buffout, 3, "%02x", byte);
        output.write(buffout, 2);
    }
}
void Hex::retransform(std::istream &input, std::ostream &output)
{
    char buff[3];
    uint8_t byte;
    while(input.read(buff, 2)){
        byte = static_cast<uint8_t>(std::stoi(buff, 0, 16));
        output.write((char*)&byte, 1);
    }
}
