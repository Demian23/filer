#include <algorithm>
#include <ios>
#include <iostream>
#include "haffman.h"
#include "bitwriter.h"

hfm::HaffmanCompression* loadhaffman(){return new hfm::HaffmanCompression;}

namespace hfm{

HaffmanHeap::HaffmanHeap(){std::make_heap(heap_.begin(), heap_.end(), compare);}

void HaffmanHeap::pushHeap(std::unique_ptr<Node>&& node)
{
    heap_.push_back(std::move(node));
    std::push_heap(heap_.begin(), heap_.end(), compare);
}

std::unique_ptr<Node> HaffmanHeap::extractMin()
{
    std::pop_heap(heap_.begin(), heap_.end(), compare);
    auto x(std::move(heap_.back()));
    heap_.pop_back();
    return x;
}


std::unique_ptr<Node> HaffmanHeap::makeTree()
{
    auto n = heap_.size();
    for(decltype(n) i = 0; i < n - 1; i++){
        auto x = extractMin();
        auto y = extractMin();
        pushHeap(std::make_unique<Node>(0, x->frequency + y->frequency, 
            std::move(x), std::move(y)));
    }
    return extractMin();
}


void HaffmanCompression::countFrequency(std::istream& input, HaffmanHeap& heap)
{
    uint64_t freq[256] = {};
    uint8_t byte;
    while(input.read((char*)(&byte), 1))
        freq[byte]++;
    for(unsigned i = 0; i < 256; i++)
        if(freq[i])
            heap.pushHeap(std::make_unique<Node>(static_cast<uint8_t>(i), freq[i]));
}

void HaffmanCompression::fillConvertTable(std::unique_ptr<Node> tree)
{
    auto& ptr = tree;
    std::string accum;
    traverse(ptr, accum);
}

void HaffmanCompression::traverse(std::unique_ptr<Node>& ptr, std::string accum)
{
    //table[byte] should be empty
    if(ptr->left == nullptr && ptr->right == nullptr){
        std::transform(accum.begin(), accum.end(), std::back_inserter(table[ptr->byte]),
            [](auto& el){return el == '1' ? true : false;});
       return;
    }
    traverse(ptr->left, accum + '0');
    traverse(ptr->right, accum + '1');
}

std::string HaffmanCompression::substitution(uint8_t byte)const
{
    auto it = table.find(byte);
    if(it != table.end()){
        std::string res;
        for(bool bit : it->second)
            res += bit ? '1' : '0';
        return res;
    }else return std::string();
}

void HaffmanCompression::transform(std::istream& input, std::ostream& output)
{
    HaffmanHeap heap;
    countFrequency(input, heap);
    auto tree = heap.makeTree();
    fillConvertTable(std::move(tree)); 

    input.clear();
    input.seekg(0);

    uint8_t byte;
    writeTable(output);
    BitWriter writer(output);
    while(input.read((char*)&byte, 1))
        writer.write(table[byte]);
}

void HaffmanCompression::writeTable(std::ostream& output)
{
    for(const auto& el : table){
        output.write((char*)&el.first, 1);
        output << substitution(el.first) << '\n';
    }
}

void HaffmanCompression::retransform(std::istream& input, std::ostream& output)
{
    restoreTable(input);
    BitReader reader(input, rotateTable());
    char buffer[512] = {};
    std::streamsize write = 0;
    while((write = static_cast<std::streamsize>(reader.read(buffer, 512))))
        output.write(buffer, write);
}

HaffmanCompression::RotatedTable HaffmanCompression::rotateTable()const
{
    RotatedTable result;
    for(const auto& el : table)
        result[el.second] = el.first;
    return result;
}

void HaffmanCompression::restoreTable(std::istream& input)
{
    enum tableParseStages{readByte, readEncoding, readOrEnd, Error};
    char byte, tempByte = 0;
    std::vector<bool> substitution;
    uint8_t currentStage = readByte;
    bool done = false;
    for(;!done;)
        switch (currentStage) {
            case readByte: input.read(&byte, 1); currentStage = readEncoding; break;
            case readEncoding: 
               tempByte = static_cast<char>(input.peek());
               if(tempByte == '0' || tempByte == '1'){
                   substitution.push_back(tempByte == '1' ? true : false);
                   input.get();
                   currentStage = readOrEnd;
               } else currentStage = Error;
               break;
            case readOrEnd:
               tempByte = static_cast<char>(input.peek());
               if(tempByte == '\n'){
                   input.get();
                   table[static_cast<uint8_t>(byte)] = substitution;
                   substitution.clear();
                   currentStage = readByte;
               } else 
                   if(tempByte == '0' || tempByte == '1'){
                       substitution.push_back(tempByte == '1' ? true : false);
                       input.get();
                       currentStage = readOrEnd;
                   } else currentStage = Error;
               break;
            case Error:
                while(substitution.size()){
                    input.putback(substitution.back() ? '1' : '0');
                    substitution.pop_back();
                }
                input.putback(byte);
                done = true;
        }
}

const char* HaffmanCompression::type()
{
    return "haffman";
}

void HaffmanCompression::setOptions(const std::string& options)
{

}

};

