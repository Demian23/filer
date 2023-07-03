#include "AutoFiler.h"


namespace fs = std::__fs::filesystem;
AutoFiler::~AutoFiler()
{
    if(transformation) delete transformation;
    if(plg_handle) dlclose(plg_handle);
}

AutoFiler::AutoFiler(const __configuration& config) : IFiler(), plg_handle(0), 
    transformation(0), transform(true)
{
    for(__configuration::const_iterator it = config.begin(); 
        it != config.end(); it++)
    {
        switch (configParamIndex(it->first)) {
            case arplg: getTransformation(it->second); break;
            case inf: in.open(it->second);break;
            case otf: out.open(it->second);break;
            case d: it->second == "to" ? transform : transform = false;break;
            case opt: options = it->second;break;
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
            throw error(cnlf);
    } else 
        throw error(cnapll);
}

void AutoFiler::process()
{
    transformation->setOptions(options);
    if(transform)
        transformation->transform(in, out);
    else 
        transformation->retransform(in, out);
}
