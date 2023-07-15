#include <algorithm>
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
    BitWriter writer(output);
    while(input.read((char*)&byte, 1))
        writer.write(table[byte]);
}

void HaffmanCompression::retransform(std::istream& input, std::ostream& output)
{

}

const char* HaffmanCompression::type()
{
    return "haffman";
}

void HaffmanCompression::setOptions(const std::string& options)
{

}

};


