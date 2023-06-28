#include <iostream>
#include "base64.h"

Base64* loadbase64(){return new Base64;}

static constexpr unsigned char encoding_table[] = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
      'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
      'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
      'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
      'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
      'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
      'w', 'x', 'y', 'z', '0', '1', '2', '3',
      '4', '5', '6', '7', '8', '9', '+', '/'
};

static unsigned char decoding_table[127] = {};

static void encode_triple(unsigned char inbuff[3], unsigned char outbuff[4])
{
    outbuff[0] = encoding_table[(inbuff[0] & 0b11111100)>>2]; 
    outbuff[1] = encoding_table[((inbuff[0] & 0b00000011) << 4) 
        | ((inbuff[1] & 0b11110000)>>4)]; 
    outbuff[2] = encoding_table[((inbuff[1] & 0b00001111)<< 2) 
        | ((inbuff[2] & 0b11000000)>>6)];
    outbuff[3] = encoding_table[inbuff[2] & 0b00111111];
}

const char* Base64::type()
{
    return "base64";
}

void Base64::transform(std::istream &input, std::ostream &output)
{
    unsigned char inbuff[3] = {};
    unsigned char outbuff[4] = {};
    while(input.read((char*)inbuff, 3)){
        encode_triple(inbuff, outbuff);
        output.write((char*)outbuff, 4);
    }
    std::streamsize last_read = input.gcount();
    if(last_read > 0 && last_read < 3){
        switch (last_read) {
            case 3:
                encode_triple(inbuff, outbuff);
                break;
            case 2:
                outbuff[0] = encoding_table[(inbuff[0] & 0b11111100)>>2]; 
                outbuff[1] = encoding_table[((inbuff[0] & 0b00000011) << 4) 
                    | ((inbuff[1] & 0b11110000)>>4)]; 
                outbuff[2] = encoding_table[(inbuff[1] & 0b00001111)<< 2];
                outbuff[3] = '=';
                break;
            case 1: 
                outbuff[0] = encoding_table[(inbuff[0] & 0b11111100)>>2]; 
                outbuff[1] = encoding_table[((inbuff[0] & 0b00000011)<< 4)];
                outbuff[2] = '=';
                outbuff[3] = '=';
                break;
            case 0:
                default: break;
        }
        output.write((char*)outbuff, 4);
    }
}

void Base64::retransform(std::istream &input, std::ostream &output)
{
    if(!decoding_table[0])
        for(unsigned char i = 0; i < sizeof(encoding_table) /sizeof(encoding_table[0]); i++)
            decoding_table[encoding_table[i]] = i;
    unsigned char inbuff[4] = {};
    unsigned char outbuff[3] = {};
    while(input.read((char*)inbuff, 4)){
        outbuff[0] = static_cast<unsigned char>(((inbuff[0] == '=' ? 0 : decoding_table[inbuff[0]]) << 2) 
            | ((inbuff[1] == '=' ? 0 : decoding_table[inbuff[1]] & 0b00110000) >> 4));
        outbuff[1] = static_cast<unsigned char>(((inbuff[1] == '=' ? 0 : decoding_table[inbuff[1]] & 0b00001111) << 4) 
            | ((inbuff[2] == '=' ? 0 : decoding_table[inbuff[2]] & 0b00111100) >> 2));
        outbuff[2] = static_cast<unsigned char>(((inbuff[2] == '=' ? 0 : decoding_table[inbuff[2]]) << 6) 
            | (inbuff[3] == '=' ? 0 : decoding_table[inbuff[3]]));
        output.write((char*)outbuff, 3);
    }
}

void Base64::setOptions(const std::string& options){}
