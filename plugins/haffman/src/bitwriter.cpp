#include<vector>
#include <iostream>
#include "bitwriter.h"

void BitWriter::write(std::vector<bool>& bits_to_write)
{
    for(bool bit : bits_to_write){
        writeByte[bitFilled++] = bit;
        if(bitFilled == 8){
            char byte = static_cast<char>(writeByte.to_ulong());
            out_.write(&byte, 1);
            bitFilled = 0;
        }
    } 
}

BitWriter::~BitWriter()
{
    // if some bytes still in, write till byte, than add three zero bytes
    // and last byte is amount of adding bites
    if(bitFilled){
        char byte = static_cast<char>(writeByte.to_ulong());
        out_.write(&byte, 1);
        byte = 0;
        out_.write(&byte, 1);
        out_.write(&byte, 1);
        out_.write(&byte, 1);
        byte = static_cast<char>(8 - bitFilled); 
        out_.write(&byte, 1);
    }
}
