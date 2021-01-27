#include "Settings_Menu_State.h"

Settings_Menu_State::Settings_Menu_State()
{
    add_button(vec2{-0.95, -0.85}, "Save", [this]() { std::cerr << "This feature has not been implemented yet." << std::endl; });
    add_button(vec2{-0.95, -0.95}, "Return", [this]() { this->set_state_flag("main"); });
}