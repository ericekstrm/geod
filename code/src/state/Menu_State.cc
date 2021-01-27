#include "Menu_State.h"

#include <iostream>

#include "settings.h"

#include "GL/gl.h"

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

    for(auto it = buttons.begin(); it != buttons.end(); it++) 
    {
        (*it)->render();
    }

    for(auto it = texts.begin(); it != texts.end(); it++) 
    {
        (*it)->render();
    }
}

void Menu_State::check_input(GLFWwindow*)
{
}

void Menu_State::activate(GLFWwindow* window)
{
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Menu_State::mouse_button_callback(int button, int action, vec2 const& position)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        for(auto it = buttons.begin(); it != buttons.end(); it++) 
        {
            (*it)->try_click(position);
        }
    }
}

void Menu_State::add_button(vec2 const& position, std::string const& text, std::function<void()> const& callback)
{
    buttons.push_back(std::make_unique<Button>(position, text, callback));
}

void Menu_State::add_text(vec2 const& position, std::string const& text)
{
    texts.push_back(std::make_unique<Text>(text, position, font));
}