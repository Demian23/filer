#include <iostream>
#include <array>
#include "rle.h"

const char* Rle::type(){return "rle";}

Rle* loadhaffman(){return new Rle;}

class FrequencyCountingStream{
public:
    FrequencyCountingStream(std::ostream& out) : out_(out){}
    void write(char byte);
    ~FrequencyCountingStream();

private:
    enum{maxByteSeq = 0x7F};
    std::ostream& out_;
    char lastByte{0};
    uint8_t repeatedSeqLen{0};
    uint8_t bufferPosition{0};
    char nonRepeatedBuffer[maxByteSeq];

    inline void writeRepeatedByte(){
        out_.write((char*)&repeatedSeqLen, 1);
        out_.write((char*)&lastByte, 1);
        repeatedSeqLen = 0;
    }
    inline void writeNonRepeatedBuffer(){
        uint8_t nonRepeatedSize = 0x80 + bufferPosition;
        out_.write((char*)&nonRepeatedSize, 1);
        out_.write(nonRepeatedBuffer, bufferPosition);
        bufferPosition = 0;
    }
};

void FrequencyCountingStream::write(char byte)
{
    if(repeatedSeqLen && byte == lastByte){
        if(bufferPosition && nonRepeatedBuffer[bufferPosition-1] == lastByte) bufferPosition--;
        if(bufferPosition) writeNonRepeatedBuffer();
        repeatedSeqLen++;
        if(repeatedSeqLen >= maxByteSeq) writeRepeatedByte();
    } else {
        if(repeatedSeqLen > 1) writeRepeatedByte();
        if(bufferPosition >= maxByteSeq) writeNonRepeatedBuffer();
        nonRepeatedBuffer[bufferPosition++] = lastByte = byte; 
        repeatedSeqLen = 1;
    }
}

FrequencyCountingStream::~FrequencyCountingStream()
{
    if(bufferPosition)
        writeNonRepeatedBuffer();
    if(repeatedSeqLen > 1) writeRepeatedByte();
}

void Rle::transform(std::istream& input, std::ostream& output)
{
    char readByte;
    FrequencyCountingStream out(output);
    while(input.read(&readByte, 1))
        out.write(readByte);
}
void Rle::retransform(std::istream& input, std::ostream& output){}
    
void Rle::setOptions(const std::string& options){}
