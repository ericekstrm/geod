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
    Text text {"Tree Simulator", vec2{-0.9, 0.9}, font};

    Button tree_growth_button {vec2{-0.9, 0.7}, "Tree Growth", [this]() { this->set_state_flag("game"); }};
    Button quit_button {vec2{-0.9, 0}, "Quit", [this]() { this->set_state_flag("quit"); }};
};
