#ifndef __BitWriter_h__
#define __BitWriter_h__

#include <iosfwd>
#include <bitset>
#include <map>
#include <vector>

class BitWriter final{
public:
    BitWriter(std::ostream& out) : out_(out), bitFilled(0){}
    void write(std::vector<bool>& bits_to_write);
    ~BitWriter();
private:
    std::ostream& out_;
    std::bitset<8> writeByte;
    uint8_t bitFilled; 
};

class BitReader final{
public:
    // cause I doubt that vector will effectively hashable in unordered_map
    using Substitutions = std::map<std::vector<bool>, uint8_t>;
    BitReader(std::istream& in, Substitutions&& sub); 
    uint64_t read(char* buffer, uint64_t size);
    uint64_t fillBitsAndWriteBytes(char* buffer, uint64_t size, 
        uint8_t meaningfullBits);
private:
    Substitutions subs_;
    std::istream& in_;
    std::vector<bool> bits;
    std::bitset<8> readByte;
    std::vector<uint8_t> bytesToWrite;
    uint64_t streamsize_;
    uint64_t currentbyte_;
};

#endif
