#pragma once

#include "Menu_State.h"
#include "Slider.h"

#include <map>

class Settings_Menu_State : public Menu_State
{
public:
    Settings_Menu_State();

    virtual std::string name() const { return "settings"; }

    virtual void render() const override;

    void add_slider(vec2 position, std::string name, float min, float max, float start);

private:

    std::map<std::string, std::unique_ptr<Slider>> sliders {};
};