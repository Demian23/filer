#ifndef __TextUI_h__
#define __TextUI_h__

#include "IUserInterface.h"
#include "Filer.h"
#include <map>
enum class CommandsCodes;
class TextUI final: public IUserInterface{
public:
    using CommandHandler = std::string(TextUI::*)(const std::string&);
    using Commands = std::map<std::string, CommandHandler>; 
    TextUI(Filer* filer); 
    void runUI()override;
    inline const char* type()override{return "text";}
    inline void setEnd(){endFlag = true;};
    std::string helpCmd(const std::string&);
    std::string quitCmd(const std::string&);
    std::string showCmd(const std::string&);
    std::string pluginsCmd(const std::string&);
    std::string runCmd(const std::string&);
    std::string delCmd(const std::string&);
private:

    TextUI(const TextUI&) = delete;
    TextUI& operator=(const TextUI&) = delete;

    Filer* filer_;
    Commands cmd; 
    bool endFlag;
};
#endif
