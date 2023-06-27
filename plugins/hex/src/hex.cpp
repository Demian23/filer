#include <cstdio>
#include <iostream>
#include <sstream>
#include "hex.h"

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
        if(spaces_)output.put(' ');
    }
}
void Hex::retransform(std::istream &input, std::ostream &output)
{
    char buff[3];
    uint8_t byte;
    while(input.read(buff, 2)){
        if(spaces_) input.get();
        byte = static_cast<uint8_t>(std::stoi(buff, 0, 16));
        output.write((char*)&byte, 1);
    }
}
void Hex::setOptions(const std::string& options)
{
    std::stringstream stream(options);
    std::string param;
    while(std::getline(stream, param, ' ')){
        if(param == "spaces")
            spaces_ = true;
    }
}
