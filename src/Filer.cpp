#include "Filer.h"
#include "TextUI.h"

Filer::Filer(const __configuration& config)
{
    __configuration::const_iterator it = config.find(param(ui));
    if(it == config.end()){
        ui_ = new TextUI(this);
    } else {
        
    }
}

void Filer::process()
{
    ui_->runUI();
}
