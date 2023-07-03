#include <iostream>
#include "TextUI.h"

static const char*const inwelcome = "::> ";

void TextUI::runUI()
{
    std::string input;
    if(!filer_->ready())
        while(std::cout << inwelcome, std::cin >> input){

        }
}
