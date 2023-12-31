#ifndef __Filer_h__
#define __Filer_h__

#include <filesystem>
#include <map>
#include <vector>
#include <string>
#include <iosfwd>

#include "IStreamTransformer.h"
#include "IUserInterface.h"
#include "ManagedPtr.h"

enum Params:char;
class Filer final{
public:
    using Configuration = std::map<Params, std::string>;
    using Path = std::filesystem::path;
    static std::unique_ptr<Filer> makeInstanceFromArgs(int argc, const char** argv);

    void run();
    bool autorun()const;
    void runTransformation();
    std::vector<Path> getAllAvailablePlugins()const;
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

    Filer(Configuration&& config) : config_(std::move(config)){}
    Filer(Configuration& config) : config_(config){}

    void loadPlugin(const std::string& plgName, const std::string& loadfName);
    void addTransformationSequence(const std::string& plgName, 
        TransformationSequence& seq);
    void transformWithSequence(TransformationSequence& seq, bool directionTo); 
    Configuration config_;
    std::map<const std::string, Transformation> transformations;
    std::map<const std::string, UsrInterface> uis;
    ManagedPtr<std::istream> in;
    ManagedPtr<std::ostream> out;
};

#endif
