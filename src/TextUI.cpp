#include <iostream>
#include "TextUI.h"
#include "IThrowable.h"

enum class CommandsCodes{Help, Quit, Plugins, Show, Run, Delete};
std::map<CommandsCodes, std::pair<std::string, std::string>> commands_strs{
    {CommandsCodes::Help, {"help", "to see this msg"}},
    {CommandsCodes::Quit, {"quit", "to quit"}},
    {CommandsCodes::Plugins, {"plugins", "to see all plugins"}},
    {CommandsCodes::Show, {"show", "to show current configuration"}},
    {CommandsCodes::Run, {"run", "to run transformation"}},
    {CommandsCodes::Delete, {"delete", "to delete configuration parameter(with args)"}}
};
static const char*const inwelcome = "::> ";
static const char*const equelness = " -> ";
static constexpr char paramDelim = ' ';
static constexpr const char* done = "Done!\n";
static const std::string& helpmsg = "Welcome in TUI of filer.\n"
    "Input one of following commands and press Enter.\n"+
    commands_strs[CommandsCodes::Help].first + equelness + commands_strs[CommandsCodes::Help].second + '\n'+
    commands_strs[CommandsCodes::Quit].first + equelness + commands_strs[CommandsCodes::Quit].second + '\n'+
    commands_strs[CommandsCodes::Plugins].first + equelness + commands_strs[CommandsCodes::Plugins].second + '\n'+
    commands_strs[CommandsCodes::Show].first + equelness + commands_strs[CommandsCodes::Show].second + '\n'+
    commands_strs[CommandsCodes::Run].first + equelness + commands_strs[CommandsCodes::Run].second + '\n' +
    commands_strs[CommandsCodes::Delete].first + equelness + commands_strs[CommandsCodes::Delete].second + '\n'
;

std::string TextUI::helpCmd(const std::string&){return helpmsg;}
std::string TextUI::quitCmd(const std::string&){endFlag = true;return "Goodbye\n";}
std::string TextUI::showCmd(const std::string&){return filer_->configStr();}
std::string TextUI::pluginsCmd(const std::string&)
{
    using path = std::__fs::filesystem::path;
    std::string res;    
    const auto plugins = filer_->getAllAvailablePlugins();
    std::for_each(plugins.begin(), plugins.end(), 
        [&res](const path& p){res += p.stem().string() + '\n';});
    return res;
}

std::string TextUI::delCmd(const std::string& p)
{
    std::string::size_type start = 0, end = 0;
    while((start = p.find_first_not_of(paramDelim, end)) != std::string::npos){
        end = p.find(paramDelim, start);    
        filer_->unsetConfigParam(p.substr(start, end-start));
    }
    return done;
}

std::string TextUI::runCmd(const std::string&)
{
    filer_->runTransformation();
    return done;
}

TextUI::TextUI(Filer* filer) : filer_(filer), endFlag(false)
{
    cmd = {
        {commands_strs[CommandsCodes::Help].first, &TextUI::helpCmd}, 
        {commands_strs[CommandsCodes::Quit].first,&TextUI::quitCmd}, 
        {commands_strs[CommandsCodes::Plugins].first,&TextUI::pluginsCmd},
        {commands_strs[CommandsCodes::Show].first, &TextUI::showCmd},
        {commands_strs[CommandsCodes::Run].first, &TextUI::runCmd},
        {commands_strs[CommandsCodes::Delete].first, &TextUI::delCmd}
    };
}

void TextUI::runUI()
{
    std::string command, params;
    while(!endFlag && (std::cout << inwelcome, std::getline(std::cin, command))){
        auto breakpos = command.find_first_of(' ');
        params = command.substr(breakpos + 1, command.size());
        command = command.substr(0, breakpos);
        auto it = cmd.find(command);
        try{
            if(it != cmd.end()){
                std::cout << (this->*(it->second))(params);
            }else{
                filer_->setConfigParam(command, params); 
            }
        } catch(IThrowable* e){
            std::cerr << e->errorDescription() + '\n';
            delete e;
        }
    }
}
