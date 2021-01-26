#pragma once

#include "Menu_State.h"

class Settings_Menu_State : public Menu_State
{
public:
    Settings_Menu_State();

    virtual std::string name() const { return "settings"; }
};