#ifndef __IStreamTransformer_h__
#define __IStreamTransformer_h__

#include <iosfwd>

class IStreamTransformer{
public:
    virtual const char* type() = 0;
    virtual void transform(std::istream& in, std::ostream& out) = 0;
    virtual void retransform(std::istream& in, std::ostream& out) = 0;
    virtual void setOptions(const std::string& options) = 0;
    virtual ~IStreamTransformer(){}
    //can add possibility of configuration using vargs
    //IStreamTransformer(const IStreamTransformer&) = delete;
    IStreamTransformer& operator=(const IStreamTransformer&) = delete;
};

typedef IStreamTransformer*(*loader)();

#define DEFINE_PLUGIN(class_type, plugin_name) \
    extern "C" {class_type* load##plugin_name();}

#endif
