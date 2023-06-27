#ifndef __Filer_h__
#define __Filer_h__

#include <map>
#include <string>

class Filer{
public:
    static Filer* makeInstanceFromArgs(int argc, char** argv);
    static std::string plgPath(const std::string& plg, const std::string& plg_dir);
    virtual bool ready() = 0;
    virtual void process() = 0;
    virtual ~Filer(){}
protected:
    typedef std::map<std::string, std::string> __configuration;
    static void parseConfigFile(const char* file, __configuration& config);
    Filer& operator=(const Filer&);
};

#endif
