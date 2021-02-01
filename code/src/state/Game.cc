#include "Game.h"
#include "Game_State.h"
#include "Settings.h"
#include "Settings_Menu_State.h"
#include "Main_Menu_State.h"

#include <GLFW/glfw3.h>
#include <ctime>
#include <ratio>
#include <chrono>

Game::Game()
{
    Settings::load();
    OpenGL_Settings::load();

    init_openGL();

    //set up input callbacks
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int){
        auto self = static_cast<Game*>(glfwGetWindowUserPointer(window));
        self->get_current_state()->key_callback(key, scancode, action);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int){

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // normalize mouse position
        vec2 position {static_cast<float>(xpos) / Settings::get_window_width() * 2 - 1, -(static_cast<float>(ypos) / Settings::get_window_height() * 2 - 1)};

        auto self = static_cast<Game*>(glfwGetWindowUserPointer(window));
        self->get_current_state()->mouse_button_callback(button, action, position);
    });

    //create states
    all_states.push_back(std::make_unique<Main_Menu_State>());
    all_states.push_back(std::make_unique<Game_State>());
    all_states.push_back(std::make_unique<Settings_Menu_State>());
    current_state = all_states.front().get();
    current_state->activate(window);
}

Game::~Game()
{
}

void Game::run()
{
    std::chrono::high_resolution_clock::time_point prev_time = std::chrono::high_resolution_clock::now();
    //srand(floor(std::chrono::duration_cast<std::chrono::seconds>( prev_time.time_since_epoch()).count()));
    while (!glfwWindowShouldClose(window))
    {
        //update logic

        std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> time_span = current_time - prev_time;

        //std::cout << "fps: " << 1000 / time_span.count() << std::endl;

        float delta_time {static_cast<float>(time_span.count() / 1000)};
        current_state->update(delta_time);
        prev_time = current_time;

        //render to the screen
        current_state->render();
        glfwSwapBuffers(window);

        //check for and handle events
        glfwPollEvents();
        current_state->check_input(window);

        //check if the state should be changed
        update_states();
    }
}

void Game::update_states()
{
    std::string new_state {current_state->update_state()};

    if (new_state != "")
    {
        if (new_state == "quit")
        {
            glfwSetWindowShouldClose(window, true);
            return;
        }

        for (auto it = all_states.begin(); it != all_states.end(); it++)
        {
            if ((*it)->name() == new_state)
            {
                current_state->deactivate(window);
                current_state = it->get();
                current_state->activate(window);

                return;
            }
        }
    }
}

void Game::init_openGL()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(Settings::get_window_width(), Settings::get_window_height(), "GEOD", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);

    //vsync off
    glfwSwapInterval( 0 );
    
    glEnable(GL_DEPTH_TEST);
}
