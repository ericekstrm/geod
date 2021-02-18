#include "Settings_Menu_State.h"

Settings_Menu_State::Settings_Menu_State()
{
    add_button(vec2{-0.95, -0.85}, "Save", [this]() { std::cerr << "This feature has not been implemented yet." << std::endl; });
    add_button(vec2{-0.95, -0.95}, "Return", [this]() { this->set_state_flag("main"); });
}

void Settings_Menu_State::render() const
{
    for(auto it = sliders.begin(); it != sliders.end(); it++) 
    {
        it->second->render();
    }
}

void Settings_Menu_State::add_slider(vec2 position, std::string name, float min, float max, float start)
{
    sliders[name] = std::make_unique<Slider>(position, min, max, start);
}
