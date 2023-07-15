#include <sstream>
#include <fstream>
#include <dlfcn.h>

#include "ErrorMsg.h"
#include "Filer.h"
#include "TextUI.h"

namespace fs = std::__fs::filesystem;
enum ErrorCodes{WrConf, WrPlgDir, WrAutorun, NoPlg, NoParam, NoFile,
    CantLoad, NoLoader, WrongDirection, NoUIPlg, DirExtFirst, NoRegFile, 
    CantOpen, NoPlugins, OneStream, NotSet};

static std::map<ErrorCodes, std::string> errorsToThrow= {
    {ErrorCodes::WrConf, "Not correct config file"},
    {ErrorCodes::WrPlgDir, "Not correct plugins directory"}, 
    {ErrorCodes::WrAutorun, "Not correct autorun value"}, 
    {ErrorCodes::NoPlg, "No such plugin available"},
    {ErrorCodes::NoParam, "Wrong config parameter"}, 
    {ErrorCodes::NoFile, "File doesn't exist"}, 
    {ErrorCodes::NoRegFile, "Not regular file"},
    {ErrorCodes::CantOpen, "Can't open file"},
    {ErrorCodes::CantLoad, "No plugin able to load"}, 
    {ErrorCodes::NoLoader, "No load function in plugin"},
    {ErrorCodes::WrongDirection, "Wrong direction(to/from)"}, 
    {ErrorCodes::NoUIPlg, "No such ui plugin"},
    {ErrorCodes::DirExtFirst, "Plugins directory and extension should be before plugins itself"},
    {ErrorCodes::NoPlugins, "No plugins set"},
    {ErrorCodes::NotSet, "not set"},
    {ErrorCodes::OneStream, "Only one stream is set"}
};

enum Params:char{PlgDir, PlgExt, Autorun, Plugins, InFile, OutFile, Direction, Options, UI};

static std::map<Params, std::string> configurationParams = {
    {Params::PlgDir, "PluginDirectory"},
    {Params::PlgExt, "PluginsExtension"},
    {Params::Plugins, "Plugins"}, 
    {Params::Autorun, "Autorun"}, 
    {Params::InFile, "InputFile"},
    {Params::OutFile, "OutputFile"}, 
    {Params::Direction, "TransformationDirection"}, 
    {Params::Options, "TransformationOptions"}, 
    {Params::UI, "UserInterface"}, 
};

static constexpr char configParamValueDelimeter = ':';
static constexpr char configParamsDelimeter = '\n';
static constexpr char configPluginsDelimeter = ' ';
static constexpr char configOptionsDelimeter = ';';

std::string findFileInDirectory(const std::string& file, const std::string& dir, 
    const std::string& ext)
{
    fs::recursive_directory_iterator begin(dir);
    fs::recursive_directory_iterator end;
    fs::recursive_directory_iterator res = std::find_if(begin, end,
        [&file, &ext](const fs::path& p)
            {return fs::is_regular_file(p) 
                    && p.filename().stem() == file 
                    && p.filename().extension() == ext;}
    );
    return res != end ? fs::absolute(res->path()).string() : ""; 
}

using HandlerF = void(*)(const std::string&, Filer::Configuration&);

void plugDirH(const std::string& dir, Filer::Configuration& config)
{
    if(fs::exists(dir) && fs::is_directory(dir)){
        config[PlgDir] = fs::absolute(dir);         
        config[PlgExt] = ".so";
    }
    else
        throw new ErrorMsg(errorsToThrow[ErrorCodes::WrPlgDir] + ": " + dir);
}
void autorunH(const std::string& val, Filer::Configuration& config)
{
    if(val == "true")
        config[Autorun] = val;
    else 
        throw new ErrorMsg(errorsToThrow[ErrorCodes::WrAutorun] + ": " + val);
}
void pluginsH(const std::string& val, Filer::Configuration& config)
{
    if(config.find(PlgDir) != config.end() && 
        config.find(PlgExt) != config.end())
    {
        config[Plugins].clear();
        std::string plgAbsolutePath;
        const auto& dir = config[PlgDir]; 
        const auto &ext = config[PlgExt];
        std::string::size_type start = 0, end = 0;
        while((start = val.find_first_not_of(configPluginsDelimeter, end)) != std::string::npos){
            end = val.find(configPluginsDelimeter, start);    
            plgAbsolutePath = findFileInDirectory(val.substr(start, end-start), dir, ext);
            if(!plgAbsolutePath.empty())
                config[Plugins] += plgAbsolutePath + configPluginsDelimeter;
            else 
                throw new ErrorMsg(errorsToThrow[ErrorCodes::NoPlg] + ": " 
                    + val);
        }
    } else 
        throw new ErrorMsg(errorsToThrow[ErrorCodes::DirExtFirst]); 
}

void infileH(const std::string& file, Filer::Configuration& config)
{
    if(fs::exists(file)){
        if(fs::is_regular_file(file))
            config[InFile] = fs::absolute(file); 
        else 
            throw new ErrorMsg(errorsToThrow[ErrorCodes::NoRegFile] + ": " + file);
   } else 
       throw new ErrorMsg(errorsToThrow[ErrorCodes::NoFile] + ": " + file);
}

void outfileH(const std::string& file, Filer::Configuration& config)
{
    if(!fs::exists(file) || fs::is_regular_file(file))
       config[OutFile] = file; 
    else 
        throw new ErrorMsg(errorsToThrow[ErrorCodes::NoRegFile] + ": " + file);
}

void directionH(const std::string& val, Filer::Configuration& config)
{
    if(val == "from" || val == "to")
        config[Direction] = val;
    else 
        throw new ErrorMsg(errorsToThrow[ErrorCodes::WrongDirection]);
}

static std::map<const std::string, HandlerF> configurationParamsHandlers = {
    {configurationParams[PlgDir], plugDirH},
    {configurationParams[PlgExt], [](const std::string&val, Filer::Configuration& config){config[Options]=val;}},
    {configurationParams[Autorun], autorunH},
    {configurationParams[Plugins], pluginsH}, 
    {configurationParams[InFile], infileH},
    {configurationParams[OutFile], outfileH}, 
    {configurationParams[Direction], directionH}, 
    {configurationParams[Options], [](const std::string&val, Filer::Configuration& config){config[Options]=val;}},
    {configurationParams[UI],[](const std::string&val, Filer::Configuration& config){config[Options]=val;}}, 
};


void Filer::verifyConfigParam(const std::string& param, 
        const std::string& value, Configuration& config)
{
    auto it = configurationParamsHandlers.find(param);
    if(it != configurationParamsHandlers.end())
        it->second(value, config);
    else 
        throw new ErrorMsg(errorsToThrow[NoParam] + ": " + param);
}

void Filer::parseConfigFile(const char* file, Configuration& config)
{
    if(fs::exists(file) && fs::is_regular_file(file)){
        std::ifstream in(file);
        if(in.good()){
            std::string param;
            while(std::getline(in, param, configParamValueDelimeter)){
                std::string value; 
                std::getline(in, value, configParamsDelimeter);
                verifyConfigParam(param, value, config);
            }
        } else 
            throw new ErrorMsg(errorsToThrow[ErrorCodes::WrConf] + ": " + file);
    } else 
        throw new ErrorMsg(errorsToThrow[ErrorCodes::WrConf] + ": " + file);
}


std::unique_ptr<Filer> Filer::makeInstanceFromArgs(int argc, char **argv)
{
    Configuration config;
    for(int i = 1; i < argc; i+=2){
        std::string temp(argv[i]);
        if(temp == "config")
            parseConfigFile(argv[i+1], config); 
        else 
            verifyConfigParam(temp, argv[i+1], config);
    }    
    return std::unique_ptr<Filer>(new Filer(std::move(config)));
}

bool Filer::autorun()const
{
    return config_.find(Autorun) != config_.end();
}

void Filer::loadPlugin(const std::string& plgName, const std::string& key)
{
    void* hndl = dlopen(plgName.c_str(), RTLD_LAZY);
    if(hndl){
        loader load = reinterpret_cast<loader>(dlsym(hndl, (std::string("load") + key).c_str()));
        if(load){
            transformations[key].first = hndl;
            transformations[key].second = load();
        }else{
            dlclose(hndl);
            throw new ErrorMsg(errorsToThrow[NoLoader] + " in " + plgName);
        }
    } else 
        throw new ErrorMsg(errorsToThrow[CantLoad] + ": " + plgName);
}

void Filer::addTransformationSequence(const std::string& plgName, TransformationSequence& seq)
{ 
    auto plgKey = fs::path(plgName).stem().string();
    const auto& it = transformations.find(plgKey);
    if(it != transformations.end())
        seq.push_back(it->second.second);
    else{
       loadPlugin(plgName, plgKey); 
       seq.push_back(transformations[plgKey].second);
    }
}

void Filer::runTransformation()
{
    if(config_.find(Plugins) != config_.end()){
        const std::string plg = config_[Plugins], options = config_[Options];
        TransformationSequence seq;
        std::ifstream stackIn;
        std::ofstream stackOut;
        std::string::size_type start = 0, end = 0, i = 0;
        while((start = plg.find_first_not_of(configPluginsDelimeter, end)) != std::string::npos){
            end = plg.find(configPluginsDelimeter, start);    
            addTransformationSequence(plg.substr(start, end-start), seq); 
        }
        if(!in && !out){
            auto inIt = config_.find(InFile), outIt = config_.find(OutFile);
            if(inIt != config_.end() && outIt != config_.end()){
                stackIn.open(config_[InFile]);
                stackOut.open(config_[OutFile]);
                if(!stackIn.good()) throw new ErrorMsg(errorsToThrow[CantOpen] + ": " + config_[InFile]);
                if(!stackOut.good()) throw new ErrorMsg(errorsToThrow[CantOpen] + ": " + config_[OutFile]);
                in = &stackIn; out = &stackOut;
            } else
               throw new ErrorMsg(
                    (inIt == config_.end() ? configurationParams[InFile] : "") + " " +
                    (outIt == config_.end() ? configurationParams[OutFile] : "") + " " +
                    errorsToThrow[NotSet]);
        }else{
            if(in)
                throw new ErrorMsg(errorsToThrow[OneStream] + ": in" );
            else 
                throw new ErrorMsg(errorsToThrow[OneStream] + ": out" );
        } 
        end = 0;
        while((start = options.find_first_not_of(configOptionsDelimeter, end)) != std::string::npos){
            end = options.find(configOptionsDelimeter, start);    
            seq[i++]->setOptions(options.substr(start, end-start)); 
        }
        auto it = config_.find(Direction);
        bool direction = it != config_.end() ? 
            (config_[Direction] == "to" ? true : false) : true;
        transformWithSequence(seq, direction);
        in = 0; out = 0;
    }else
        throw new ErrorMsg(errorsToThrow[NoPlugins]);
}

void Filer::transformWithSequence(TransformationSequence& seq, bool directionTo)
{
    std::stringstream tempstreams[2];
    switch (seq.size()) {
        case 1:
            if(directionTo)
                seq[0]->transform(*in, *out);
            else 
                seq[0]->retransform(*in, *out);
            break;
        case 2:
            if(directionTo){
                seq[0]->transform(*in, tempstreams[0]);
                seq[1]->transform(tempstreams[0], *out);
            }else{
                seq[0]->retransform(*in, tempstreams[0]);
                seq[1]->retransform(tempstreams[0], *out);
            }
            break;
        default:
            std::vector<TransformationSequence>::size_type i(1);
            if(directionTo){
                seq[0]->transform(*in, tempstreams[0]);
                for(; i < seq.size()-1; i++)
                    seq[i]->transform(tempstreams[(i % 2 + 1) % 2], tempstreams[i % 2]); 
                seq[i + 1]->transform(tempstreams[i % 2], *out);
            }else{
                seq[0]->retransform(*in, tempstreams[0]);
                for(; i < seq.size()-1; i++)
                    seq[i]->retransform(tempstreams[(i % 2 + 1) % 2], tempstreams[i % 2]); 
                seq[i + 1]->retransform(tempstreams[i % 2], *out);
            }
            break;
    }
}

Filer::~Filer()
{
    for(const auto& transformation : transformations){
        delete transformation.second.second;
        if(transformation.second.first)dlclose(transformation.second.first);
    } 
    for(const auto& ui : uis){
        delete ui.second.second;
        if(ui.second.first)dlclose(ui.second.first);
    }
}

void Filer::run()
{
    auto it = config_.find(UI);
    if(it != config_.end()){
        uis[it->second].second->runUI();
    }else{
        if(uis.size())
            config_[UI] = uis[0].second->type();
        else{
            TextUI* ui = new TextUI(this);
            uis[ui->type()] = std::make_pair(nullptr, ui);
            config_[UI] =  ui->type();
        }
        uis[config_[UI]].second->runUI();
    }
}

std::string Filer::configStr()const
{
    std::string result;    
    std::for_each(config_.begin(), config_.end(), 
        [&result](const auto& param){result += configurationParams[param.first] 
            + "=>" + param.second + '\n';});
    return result;
}

std::vector<fs::path> Filer::getAllAvailablePlugins()const
{
    auto dir_it = config_.find(PlgDir);
    std::vector<fs::path> res;
    if(dir_it != config_.end()){
        const std::string& ext = config_.find(PlgExt)->second;
        std::copy_if(
            fs::recursive_directory_iterator(dir_it->second), 
            fs::recursive_directory_iterator(), 
            std::back_inserter(res),
            [&ext](const fs::path& dir_el){return dir_el.extension() == ext;}
        );
    } else 
       throw new ErrorMsg(configurationParams[PlgDir] + " " + 
            errorsToThrow[NotSet]);
    return res;
}

void Filer::unsetConfigParam(const std::string& param)
{
    auto it = std::find_if(
                configurationParams.begin(), 
                configurationParams.end(), 
                [&param](const auto& p){return p.second == param;});
    if(it != configurationParams.end())
        config_.erase(it->first);
    else 
        throw new ErrorMsg(errorsToThrow[NoParam] + ": " + param);
}
