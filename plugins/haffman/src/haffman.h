#ifndef __Haffman_h__
#define __Haffman_h__

#include "../../../src/IStreamTransformer.h"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <map>
#include <memory>
#include <vector>

namespace hfm{

struct Node{
    uint8_t byte;
    uint64_t frequency; 
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    Node(uint8_t b = 0, uint64_t f = 0, std::unique_ptr<Node>&& l = 0, std::unique_ptr<Node>&& r = 0)
        : byte(b), frequency(f), left(std::move(l)), right(std::move(r)){}
};

class HaffmanHeap 
{
public:
    HaffmanHeap();
    std::unique_ptr<Node> extractMin();
    void pushHeap(std::unique_ptr<Node>&& node);
    std::unique_ptr<Node> makeTree();
    static bool compare(const std::unique_ptr<Node>& f, const std::unique_ptr<Node> &s)
    {return f->frequency > s->frequency;}
private:
    std::vector<std::unique_ptr<Node>> heap_;
};


class HaffmanCompression : public IStreamTransformer{
public:

    const char* type()override;
    void transform(std::istream& input, std::ostream& output)override; 
    void retransform(std::istream& input, std::ostream& output)override; 
    void setOptions(const std::string& options)override;

    using ConvertTable = std::unordered_map<uint8_t, std::vector<bool>>;
    using RotatedTable = std::map<std::vector<bool>, uint8_t>;
    void countFrequency(std::istream& input, HaffmanHeap& heap);
    void fillConvertTable(std::unique_ptr<Node> tree);
    void traverse(std::unique_ptr<Node>& ptr, std::string accum); 
    std::string substitution(uint8_t byte)const;
    void writeTable(std::ostream& output);
    void restoreTable(std::istream& input);
    RotatedTable rotateTable()const; 
private:
    ConvertTable table;
};

};

DEFINE_PLUGIN(hfm::HaffmanCompression, haffman)

#endif
