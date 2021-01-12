#pragma once

#if (defined (_WIN32) || defined (_WIN64))
    #include "glad/glad.h"
#else
    #include "GL/gl.h"
#endif

#include "GLFW/glfw3.h"
#include <list>
#include <vector>
#include "State.h"
#include <memory>

#include "Model_Shader.h"

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    void update_states();

    void init_openGL();
    //TODO: void destroyOpenGL();

    GLFWwindow* window;

    std::vector<std::unique_ptr<State>> all_states {};
    State* current_state;
};
