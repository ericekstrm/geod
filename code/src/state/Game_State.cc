#include "Game_State.h"
#include "Flying_Camera.h"
#include "Third_Person_Camera.h"
#include "settings.h"

#include <iostream>
#include <ctime>

Game_State::Game_State()
{
    srand(time(0));

    camera = std::make_unique<Flying_Camera>(vec3{20, 10, 20}, vec3{-1, 0, -1});
}

Game_State::~Game_State()
{
}

void Game_State::update(float delta_time)
{
    for (auto it = models.begin(); it != models.end(); it++)
    {
        it->update(delta_time);
    }
    camera->update(delta_time);

    lights.update(delta_time);
    shadowmap.set_light_position(lights.get_sun_position());
}

void Game_State::render() const
{
    renderer.render_to_shadowmap(shadowmap, &road, &terrain, &m);

    main_fbo.bind();
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.render_skybox(skybox, camera.get());

    renderer.render(camera.get(), lights, shadowmap, &m, &road, &terrain);

    lights.render(camera->get_camera_matrix());
    main_fbo.unbind();

    renderer.render_godray(sun_framebuffer, lights, camera.get(), &m, &road, &terrain);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    main_image.render(lights.get_sun_screen_position(camera.get()));
}

void Game_State::check_input(GLFWwindow * window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        change_state = "main";
    }

    camera->check_input(window);
}
void Game_State::activate(GLFWwindow* window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetCursorPos(window, static_cast<double>(window_width / 2.0), static_cast<double>(window_height / 2.0));
}

void Game_State::deactivate(GLFWwindow*)
{

}