#include "Menu_State.h"

#include <iostream>

#include "settings.h"

#ifdef WIN32
    #include "glad/glad.h"
#else
    #include "GL/gl.h"
#endif

Menu_State::Menu_State()
{
}

Menu_State::~Menu_State()
{
}

void Menu_State::update(float)
{
}

void Menu_State::render() const
{
    glClear(GL_COLOR_BUFFER_BIT);

    text.render();

    tree_growth_button.render();
    quit_button.render();
}

void Menu_State::check_input(GLFWwindow * window)
{
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        change_state = "game";
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // normalize mouse position
        xpos = xpos / window_width * 2 - 1;
        ypos = -(ypos / window_height * 2 - 1);

        tree_growth_button.try_click(vec2{static_cast<float>(xpos), static_cast<float>(ypos)});
        quit_button.try_click(vec2{static_cast<float>(xpos), static_cast<float>(ypos)});
    }
}

void Menu_State::activate(GLFWwindow* window)
{
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Menu_State::deactivate(GLFWwindow*)
{
}