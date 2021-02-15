#pragma once

#include "GL/gl.h"

#include "GLFW/glfw3.h"
#include <list>
#include <vector>
#include "State.h"
#include <memory>

class Game
{
public:
    Game();
    ~Game();

    void run();

    State* get_current_state() {return current_state; };

private:
    void update_states();

    void init_openGL();
    //TODO: void destroyOpenGL();

    GLFWwindow* window;

    std::vector<std::unique_ptr<State>> all_states {};
    State* current_state;
};
