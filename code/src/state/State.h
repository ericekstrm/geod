#pragma once

#include <GLFW/glfw3.h>
#include <string>

#include "Vector.h"

class State
{
public:
    State() {}
    virtual ~State() {}

    virtual void update(float delta_time) = 0;
    virtual void render() const = 0;
    virtual void check_input(GLFWwindow* window) = 0;
    virtual std::string name() const = 0;

    // Runs right before and after switching state
    virtual void activate(GLFWwindow*) {}
    virtual void deactivate(GLFWwindow*) {}

    void set_state_flag(std::string const& new_state) { change_state = new_state; }
    std::string update_state()
    {
        std::string tmp {change_state};
        change_state = "";
        return tmp;
    }

    //input callbacks
    virtual void key_callback(int, int, int) {}; // (int key, int scancode, int action)
    virtual void mouse_button_callback(int, int, vec2 const&) {}; // (int button, int action, vec2 const& position)

protected:

    // Flag for changing state. Set to the name of another 
    // state to switch state in the next loop.
    std::string change_state {};
};
