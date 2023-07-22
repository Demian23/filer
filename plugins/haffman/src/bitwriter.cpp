#include <climits>
#include <vector>
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
    // if some bytes still in, write till byte, than add two zero bytes
    // and last byte is amount of usefull bites
    if(bitFilled){
        char byte = static_cast<char>(writeByte.to_ulong());
        out_.write(&byte, 1);
        byte = 0;
        out_.write(&byte, 1);
        out_.write(&byte, 1);
        byte = static_cast<char>(bitFilled); 
        out_.write(&byte, 1);
    }
}

BitReader::BitReader(std::istream& in, Substitutions&& sub)
    : subs_(std::move(sub)), in_(in), currentbyte_(0)
{
    auto savePos = in_.tellg();
    in_.seekg(0, std::ios::end); 
    streamsize_ = static_cast<uint64_t>(in_.tellg() - savePos);
    in_.seekg(savePos, std::ios::beg);
}

uint64_t BitReader::read(char* buffer, uint64_t size)
{
    uint64_t writeSize = 0;
    uint8_t byte, significantBits = 0;
    if(!bytesToWrite.empty()){
        for(;writeSize < size && writeSize < bytesToWrite.size(); writeSize++)
            buffer[writeSize] = static_cast<char>(bytesToWrite[writeSize]);
        bytesToWrite.erase(bytesToWrite.begin(),
            bytesToWrite.begin() + static_cast<int64_t>(writeSize));
        
    }

    while(writeSize < size && in_.read((char*)&byte, 1)){
        readByte = byte;   
        if(currentbyte_ !=  streamsize_ - 4){
            significantBits = 8; 
        } else {
            char peekBuffer[3] = {};
            in_.read(peekBuffer, 3);
            if(peekBuffer[0] == '\0' && peekBuffer[1] == '\0'){
                significantBits = static_cast<uint8_t>(peekBuffer[2]);
                currentbyte_ += 3;
            }else{
               in_.putback(peekBuffer[0]); 
               in_.putback(peekBuffer[1]); 
               in_.putback(peekBuffer[2]); 
            }
        }
        currentbyte_++;
        writeSize
            += fillBitsAndWriteBytes(buffer + writeSize, size - writeSize, 
            significantBits);
    }
    return writeSize;
}

uint64_t BitReader::fillBitsAndWriteBytes(char* buffer, uint64_t size, 
    uint8_t meaningfullBits)
{
    uint64_t result = 0;
    for(uint64_t i = 0; i < meaningfullBits; i++){
        bits.push_back(readByte[i]);
        if(subs_.find(bits) != subs_.end()){
            if(result < size){
                buffer[result++] = static_cast<char>(subs_[bits]);
            } else 
                bytesToWrite.push_back(subs_[bits]);
            bits.clear();
        }
    }
    return result;
}
