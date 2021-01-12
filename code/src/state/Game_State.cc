#include "Game_State.h"
#include "Flying_Camera.h"
#include "Third_Person_Camera.h"
#include "settings.h"

#include <iostream>
#include <ctime>

Game_State::Game_State()
{

    srand(time(0));

    camera = std::make_unique<Camera>(vec3{40, 20, 40}, vec3{-1, 0, -1});
    
    //lights.add_pos_light(vec3{0, 5, 0}, vec3{1, 0.85, 0.7});

    trees.push_back(Tree{vec3{0, 0, 0}});
        
    for (int j = 0; j < 10; j++)
    {
        trees[0].update(0);
    }

    /*for(int i = 0; i < 128; i++)
    {
        trees.push_back(Tree{vec3{rand() % 128 - 64, 0, rand() % 128 - 64}});
        
        for (int j = 0; j < 10; j++)
        {
            trees[i].update(0);
        }
    }*/
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

    terrain.update(camera->get_position());
}

void Game_State::render() const
{
    render_to_shadowmap();

    main_fbo.bind();
    render_scene();
    main_fbo.unbind();

    sun_framebuffer.bind();
    render_godray_scene();
    sun_framebuffer.unbind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    main_image.render(lights.get_sun_screen_position(camera.get()));
}

void Game_State::render_scene() const
{
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skybox_shader.start();
    skybox_shader.load_projection_matrix();
    skybox_shader.load_camera_matrix(camera->get_camera_matrix().remove_translation());
    skybox.render();
    skybox_shader.stop();

    //render normal
    shader.start();
    shader.load_projection_matrix();
    shader.load_camera_matrix(camera->get_camera_matrix());
    shader.load_camera_position(camera->get_position());
    shader.load_lights(lights);
    shader.load_light_space_matrix(shadowmap.get_position());

    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, shadowmap.get_texture_id());

    for (auto it = models.begin(); it != models.end(); it++)
    {
        it->render(&shader);
    }

    terrain.render(&shader);

    for (auto it = trees.begin(); it != trees.end(); it++)
    {
        it->render(&shader);
    }

    shader.stop();

    for (auto it = trees.begin(); it != trees.end(); it++)
    {
        it->render_leafs(camera.get(), &lights);
    }

    lights.render(camera->get_camera_matrix());
}

void Game_State::render_godray_scene() const
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lights.render_sun(camera.get());

    god_ray_shader.start();
    god_ray_shader.load_projection_matrix();
    god_ray_shader.load_camera_matrix(camera->get_camera_matrix());

    for (auto it = models.begin(); it != models.end(); it++)
    {
        it->render(&god_ray_shader);
    }

    terrain.render(&god_ray_shader);

    for (auto it = trees.begin(); it != trees.end(); it++)
    {
        it->render(&god_ray_shader);
    }

    god_ray_shader.stop();

    god_ray_leaf_shader.start();
    god_ray_leaf_shader.load_projection_matrix();
    god_ray_leaf_shader.load_camera_matrix(camera->get_camera_matrix());
    for (auto it = trees.begin(); it != trees.end(); it++)
    {
        it->render_leafs(&god_ray_leaf_shader);
    }
    god_ray_leaf_shader.stop();
}

void Game_State::render_to_shadowmap() const
{
    shadowmap.activate();
    glCullFace(GL_FRONT);
    shadowmap.render(terrain);
    for (auto it = models.begin(); it != models.end(); it++)
    {
        shadowmap.render(*it);
    }

    for (auto it = trees.begin(); it != trees.end(); it++)
    {
        shadowmap.render(*it);
    }
    
    glCullFace(GL_BACK);
    shadowmap.deactivate();
}

void Game_State::check_input(GLFWwindow * window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        change_state = "menu";
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        for (auto it = trees.begin(); it != trees.end(); it++)
        {
            it->update(0);
        }
    }

    camera->check_input(window);
}
void Game_State::activate(GLFWwindow* window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetCursorPos(window, static_cast<double>(window_width / 2.0), static_cast<double>(window_height / 2.0));

    // Exempel av hur man skulle kunna hantera enskilda knapptryckningar
    /*glfwSetKeyCallback(window, key_callback);
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode){
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            auto self = static_cast<Game_State*>(glfwGetWindowUserPointer(window));
            self->set_state_flag("menu");
        }
    });*/
}

void Game_State::deactivate(GLFWwindow*)
{

}