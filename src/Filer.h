#ifndef __Filer_h__
#define __Filer_h__

#include <filesystem>
#include <map>
#include <vector>
#include <string>
#include <iosfwd>

#include "IStreamTransformer.h"
#include "IUserInterface.h"

enum Params:char;
class Filer final{
public:
    using Configuration = std::map<Params, std::string>;
    static Filer* makeInstanceFromArgs(int argc, char** argv);
    void run();
    void runTransformation();
    std::vector<std::__fs::filesystem::path> getAllAvailablePlugins()const;
    bool autorun()const;
    std::string configStr()const;
    inline void setConfigParam(const std::string& param, const std::string& value)
    {verifyConfigParam(param, value, config_);}
    void unsetConfigParam(const std::string& param);
    ~Filer();
private:
    using Transformation = std::pair<void*, IStreamTransformer*>;
    using UsrInterface = std::pair<void*, IUserInterface*>;
    using TransformationSequence = std::vector<IStreamTransformer*>;

    static void parseConfigFile(const char* file, Configuration& config);
    static void verifyConfigParam(const std::string& param, 
        const std::string& value, Configuration& config);
// no copying and assigning
    Filer& operator=(const Filer&) = delete;
    Filer(const Filer&) = delete;

    Filer(Configuration&& config) : config_(config){}
    Filer(Configuration& config) : config_(config){}

    void loadPlugin(const std::string& plgName, const std::string& loadfName);
    void addTransformationSequence(const std::string& plgName, 
        TransformationSequence& seq);
    void transformWithSequence(TransformationSequence& seq, bool directionTo); 
    Configuration config_;
    std::map<const std::string, Transformation> transformations;
    std::map<const std::string, UsrInterface> uis;
    std::istream* in{0};
    std::ostream* out{0};
};

#endif
