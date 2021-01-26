#pragma once

#include "Menu_State.h"

class Main_Menu_State : public Menu_State
{
public:
    Main_Menu_State();

    virtual std::string name() const { return "main"; }
};