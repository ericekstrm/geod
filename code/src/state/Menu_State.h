#pragma once

#include "State.h"
#include "Text.h"
#include "Button.h"
#include "Image.h"

class Menu_State : public State
{
public:
    Menu_State();
    ~Menu_State();

    virtual void update(float delta_time);
    virtual void render() const;
    virtual void check_input(GLFWwindow* window);
    virtual std::string name() const { return "menu"; }

    void activate(GLFWwindow* window) override;
    void deactivate(GLFWwindow* window) override;

private:
    Font font {"arial", 48};
    Text text {"Tree Simulator", vec2{0, -1}, font};

    Button tree_growth_button {vec2{-1, -1}, "Tree Growth", [this]() { this->set_state_flag("game"); }};
    Button auit_button {vec2{-0.9, 0.1}, "Auit", [this]() { this->set_state_flag("quit"); }};
    Button buit_button {vec2{-0.9, -0.1}, "Buit", [this]() { this->set_state_flag("quit"); }};
    Button cuit_button {vec2{-0.9, -0.3}, "Cuit", [this]() { this->set_state_flag("quit"); }};
    Button duit_button {vec2{-0.9, -0.5}, "Duit", [this]() { this->set_state_flag("quit"); }};
    Button euit_button {vec2{-0.9, -0.7}, "Euit", [this]() { this->set_state_flag("quit"); }};
};
