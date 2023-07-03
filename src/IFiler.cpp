#include "IFiler.h"
#include "IStreamTransformer.h"
#include "AutoFiler.h"
#include "Filer.h"

static const char*const error_msgs[] = {"Not correct config file", 
    "Not correct plugins directory", "Not correct autorun value", 
    "No such plugin available for autorun", "Wrong config parameter", 
    "Wrong input file", "No plugin able to load", "No load function in plugin",
    "Wrong direction(to/from)", "No such ui plugin"};
static const char*const config_params[] = {"plugin_directory", 
    "autorun", "autorun_plugins", "input_file", "output_file", "direction", 
    "options", "ui"};

int IFiler::configParamIndex(const std::string& param)
{
    const char*const* pointer_to_param = std::find(config_params, 
        config_params + sizeof(config_params), param);
    return static_cast<int>(pointer_to_param - config_params);
}

const char*const IFiler::error(error_codes error_code){return error_msgs[error_code];}
const char*const IFiler::param(params param_index){ return config_params[param_index];}

namespace fs = std::__fs::filesystem;
std::string IFiler::plgPath(const std::string& plg, const std::string& plg_dir)
{
    fs::recursive_directory_iterator begin(plg_dir);
    fs::recursive_directory_iterator end;
    fs::recursive_directory_iterator res = std::find_if(begin, end,
        [&plg](const fs::path& p)
            {return fs::is_regular_file(p) && p.filename().stem() == plg;}
    );
    return res != end ? fs::absolute(res->path()).string() : ""; 
}

void IFiler::parseConfigFile(const char* file, __configuration& config)
{
    if(fs::exists(file) && fs::is_regular_file(file)){
        std::ifstream in(file);
        std::string plg_dir, plg_path, param;
        while(std::getline(in, param, ':')){
            std::string value; 
            std::getline(in, value);
            switch (configParamIndex(param)) {
                case plgd:
                    if(fs::exists(value.c_str()) && fs::is_directory(value.c_str())){
                        plg_dir = value;
                        config.emplace(param, value);        
                    } else{
                        throw error_msgs[cplgdir];
                    }
                    break;
                case ar:
                    if(value == "true")
                        config.emplace(param, value);
                    else 
                        throw error_msgs[cvalautorun];
                    break;
                case arplg:
                    plg_path = plgPath(value, plg_dir);
                    if(!plg_path.empty())
                        config.emplace(param, plg_path);
                    else 
                        throw error_msgs[cnosuchplg];
                    break;
                case inf:
                    if(fs::exists(value) && fs::is_regular_file(value))
                        config.emplace(param, value);
                    else 
                        throw error_msgs[cwrin];
                    break;
                case otf:
                    config.emplace(param, value);
                    break;
                case d:
                    if(value == "from" || value == "to")
                        config.emplace(param, value);
                    else 
                        throw error_msgs[cwdirect];
                    break;
                case opt:
                    config.emplace(param, value);
                    break;
                case ui:
                    plg_path = plgPath(value, plg_dir);
                    if(!plg_path.empty())
                        config.emplace(param, plg_path);
                    else 
                        throw error_msgs[cnouiplg];
                    break;
                default:
                    throw error_msgs[cnosuchpar];
            }
        }
    } else 
        throw error_msgs[cconfig];
}


IFiler* IFiler::makeInstanceFromArgs(int argc, char **argv)
{
    __configuration config;
    for(int i = 1; i < argc; i+=2){
        std::string temp(argv[i]);
        if(temp == "config"){
            parseConfigFile(argv[i+1], config); 
        }
    }    
    if(config.find(config_params[ar]) != config.end())
        return new AutoFiler(config);
    else 
        return new Filer(config);
}

