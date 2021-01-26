#include "Main_Menu_State.h"

Main_Menu_State::Main_Menu_State()
{
    add_button(vec2{-0.95, 0.7}, "Run", [this]() { this->set_state_flag("game"); });
    add_button(vec2{-0.95, 0.5}, "Settings", [this]() { this->set_state_flag("settings"); });
    add_button(vec2{-0.95, 0.3}, "Quit", [this]() { this->set_state_flag("quit"); });
}