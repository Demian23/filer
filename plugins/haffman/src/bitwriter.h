#ifndef __BitWriter_h__
#define __BitWriter_h__

#include <iosfwd>
#include <bitset>

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

#endif
