#include <filesystem>
#include <fstream>
#include <dlfcn.h>
#include "Filer.h"
#include "IStreamTransformer.h"

enum error_codes{cconfig, cplgdir, cvalautorun, cnosuchplg, cnosuchpar, cwrin,
    cnapll, cnlf};
static constexpr const char*const error_msgs[] = {"Not correct config file", 
    "Not correct plugins directory", "Not correct autorun value", 
    "No such plugin available for autorun", "Wrong config parameter", 
    "Wrong input file", "No plugin able to load", "No load function in plugin"};
static constexpr const char*const config_params[] = {"plugin_directory", 
    "autorun", "autorun_plugins", "input_file", "output_file"};

namespace fs = std::__fs::filesystem;
std::string Filer::plgPath(const std::string& plg, const std::string& plg_dir)
{
    fs::recursive_directory_iterator begin(plg_dir);
    fs::recursive_directory_iterator end;
    fs::recursive_directory_iterator res = std::find_if(begin, end,
        [&plg](const fs::path& p)
            {return fs::is_regular_file(p) && p.filename().stem() == plg;}
    );
    return res != end ? fs::absolute(res->path()).string() : ""; 
}

void Filer::parseConfigFile(const char* file, __configuration& config)
{
    if(fs::exists(file) && fs::is_regular_file(file)){
        std::ifstream in(file);
        std::string plg_dir, plg_path, param;
        while(std::getline(in, param, ':')){
            std::string value; 
            std::getline(in, value);
            const char*const* pointer_to_param = 
                std::find(config_params, 
                        config_params + sizeof(config_params), 
                        param);
            switch (pointer_to_param - config_params) {
                case 0:
                    if(fs::exists(value.c_str()) && fs::is_directory(value.c_str())){
                        plg_dir = value;
                        config.emplace(param, value);        
                    } else{
                        throw error_msgs[cplgdir];
                    }
                    break;
                case 1:
                    if(value == "true")
                        config.emplace(param, value);
                    else 
                        throw error_msgs[cvalautorun];
                    break;
                case 2:
                    //check wether plugin exists
                    //if not throw exception
                    plg_path = plgPath(value, plg_dir);
                    if(!plg_path.empty())
                        config.emplace(param, plg_path);
                    else 
                        throw error_msgs[cnosuchplg];
                    break;
                case 3:
                    if(fs::exists(value) && fs::is_regular_file(value))
                        config.emplace(param, value);
                    else 
                        throw error_msgs[cwrin];
                    break;
                case 4:
                    config.emplace(param, value);
                    break;
                default:
                    throw error_msgs[cnosuchpar];
            }
        }
    } else 
        throw error_msgs[cconfig];
}

class AutoFiler: public Filer{
public:
    void process()override;
    bool ready()override{return true;}
    AutoFiler(const __configuration& config);
    virtual ~AutoFiler(){if(plg_handle)dlclose(plg_handle); 
        if(transformation) delete transformation;}
private:
    std::ifstream in;
    std::ofstream out;
    void* plg_handle;
    IStreamTransformer* transformation;
    AutoFiler(const AutoFiler&);     
    void getTransformation(const std::string& plg_way);
};

Filer* Filer::makeInstanceFromArgs(int argc, char **argv)
{
    __configuration config;
    for(int i = 1; i < argc; i+=2){
        std::string temp(argv[i]);
        if(temp == "config"){
            parseConfigFile(argv[i+1], config); 
        }
    }    
    if(config.find(config_params[1]) != config.end())
        return new AutoFiler(config);
    else return 0;
}

AutoFiler::AutoFiler(const __configuration& config) : Filer(), plg_handle(0), transformation(0)
{
    for(__configuration::const_iterator it = config.begin(); 
        it != config.end(); it++)
    {
        const char*const* pointer_to_param = std::find(config_params, 
            config_params + sizeof(config_params), it->first);
        switch (pointer_to_param - config_params) {
            case 2: getTransformation(it->second); break;
            case 3: in.open(it->second);
            case 4: out.open(it->second);
            default:break;
        }
    }
}

void AutoFiler::getTransformation(const std::string& plg_way)
{
    fs::path temp = plg_way;
    plg_handle = dlopen(plg_way.c_str(), RTLD_LAZY);
    if(plg_handle){
        loader load = reinterpret_cast<loader>(dlsym(plg_handle, 
            (std::string("load") + temp.stem().c_str()).c_str()));
        if(load)
            transformation = load();
        else 
            // possible leak 
            throw error_msgs[cnlf];
    } else 
        throw error_msgs[cnapll];
}

void AutoFiler::process()
{
    // path direction of transformation
    transformation->transform(in, out);
}
