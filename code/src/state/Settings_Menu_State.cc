#include "Settings_Menu_State.h"

Settings_Menu_State::Settings_Menu_State()
{
    add_button(vec2{-0.95, -0.85}, "Save", [this]() { std::cout << "hej alice!!"; });
    add_button(vec2{-0.95, -0.95}, "Return", [this]() { this->set_state_flag("main"); });
}