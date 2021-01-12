#include "Game.h"
#include "Game_State.h"
#include "Menu_State.h"
#include "settings.h"

#include <GLFW/glfw3.h>
#include <ctime>
#include <ratio>
#include <chrono>

Game::Game()
{
    init_openGL();

    all_states.push_back(std::make_unique<Menu_State>());
    all_states.push_back(std::make_unique<Game_State>());
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

    window = glfwCreateWindow(window_width, window_height, "TSBK03 Tree Simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    //vsync off
    glfwSwapInterval( 0 );

    #ifdef WIN32
        gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    #endif
    
    glEnable(GL_DEPTH_TEST);
}
