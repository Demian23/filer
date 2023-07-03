#ifndef __IFiler_h__
#define __IFiler_h__

#include <map>
#include <string>
#include <filesystem>
#include <fstream>
#include <dlfcn.h>

class IFiler{
public:
    static IFiler* makeInstanceFromArgs(int argc, char** argv);
    static std::string plgPath(const std::string& plg, const std::string& plg_dir);
    virtual bool ready() = 0;
    virtual void process() = 0;
    virtual ~IFiler(){}
protected:
    enum error_codes{cconfig, cplgdir, cvalautorun, cnosuchplg, cnosuchpar, cwrin,
        cnapll, cnlf, cwdirect, cnouiplg};
    enum params{plgd, ar, arplg, inf, otf, d, opt, ui};
    typedef std::map<std::string, std::string> __configuration;
    static void parseConfigFile(const char* file, __configuration& config);
    static int configParamIndex(const std::string& param);
    static const char*const error(error_codes err_code);
    static const char*const param(params param_index);
    IFiler& operator=(const IFiler&);
};

#endif
