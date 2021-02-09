#include "Game_State.h"

#include "Flying_Camera.h"
#include "Settings.h"

#include <iostream>

Game_State::Game_State()
{
    srand(time(0));

    camera = std::make_unique<Flying_Camera>(vec3{20, 10, 20}, vec3{-1, 0, -1});

    lights.add_pos_light(vec3{35,6,5}, vec3 {1,1,1});


    //PBR temp
    model::Material mat {c1.get_material()};

    //grass
    mat.texture_id = model::load_texture("res/textures/grass1-bl/grass1-albedo3.png");
    mat.normal_map = model::load_texture("res/textures/grass1-bl/grass1-normal1-ogl.png");
    mat.metallic_map = model::load_texture("res/textures/grass1-bl/grass1-rough.png");
    mat.roughness_map = model::load_texture("res/textures/grass1-bl/grass1-rough.png");
    mat.ao_map = model::load_texture("res/textures/grass1-bl/grass1-ao.png");
    c1.set_material(mat);

    //asphalt
    mat.texture_id = model::load_texture("res/textures/pebbled-asphalt1-bl/pebbled_asphalt_albedo.png");
    mat.normal_map = model::load_texture("res/textures/pebbled-asphalt1-bl/pebbled_asphalt_Normal-ogl.png");
    mat.metallic_map = model::load_texture("res/textures/pebbled-asphalt1-bl/pebbled_asphalt_Metallic.png");
    mat.roughness_map = model::load_texture("res/textures/pebbled-asphalt1-bl/pebbled_asphalt_Roughness.png");
    mat.ao_map = model::load_texture("res/textures/pebbled-asphalt1-bl/pebbled_asphalt_ao.png");
    c2.set_material(mat);

    //tile
    mat.texture_id = model::load_texture("res/textures/bathroomtile1-bl/bathroomtile1_basecolor.png");
    mat.normal_map = model::load_texture("res/textures/bathroomtile1-bl/bathroomtile1_normal-ogl.png");
    mat.metallic_map = model::load_texture("res/textures/bathroomtile1-bl/bathroomtile1_metalness.png");
    mat.roughness_map = model::load_texture("res/textures/bathroomtile1-bl/bathroomtile1_roughness.png");
    mat.ao_map = model::load_texture("res/textures/bathroomtile1-bl/bathroomtile1_ao.png");
    c3.set_material(mat);

    //metal
    mat.texture_id = model::load_texture("res/textures/beaten-up-metal1-bl/beaten-up-metal1-albedo.png");
    mat.normal_map = model::load_texture("res/textures/beaten-up-metal1-bl/beaten-up-metal1-Normal-ogl.png");
    mat.metallic_map = model::load_texture("res/textures/beaten-up-metal1-bl/beaten-up-metal1-Metallic.png");
    mat.roughness_map = model::load_texture("res/textures/beaten-up-metal1-bl/beaten-up-metal1-Roughness.png");
    mat.ao_map = model::load_texture("res/textures/beaten-up-metal1-bl/beaten-up-metal1-ao.png");
    c4.set_material(mat);
}

Game_State::~Game_State()
{
}

void Game_State::update(float delta_time)
{
    camera->update(delta_time);

    lights.update(delta_time);
    shadowmap.set_light_position(lights.get_sun_position());

    if (show_debug)
    {
        //exponential moving average for fps counter
        float alpha = 0.01;
        fps_accumulator = (alpha / delta_time) + (1 - alpha) * fps_accumulator;
        debug_fps.set_text("FPS: " + std::to_string(static_cast<int>(fps_accumulator)));
    }
}

void Game_State::render() const
{
    renderer.render_to_shadowmap(shadowmap, {&terrain});
    renderer.render_to_shadowmap(shadowmap, scene);

    main_fbo.bind();
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.render_skybox(skybox, camera.get());

    renderer.render(camera.get(), lights, shadowmap, {&terrain});
    renderer.render(camera.get(), lights, shadowmap, scene);
    renderer.render_PBR(camera.get(), lights, shadowmap, {&c1, &c2, &c3, &c4});

    lights.render(camera->get_camera_matrix());
    main_fbo.unbind();

    renderer.render_godray(sun_framebuffer, lights, camera.get(), {&terrain});
    renderer.render_godray(sun_framebuffer, lights, camera.get(), scene);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    main_image.render(lights.get_sun_screen_position(camera.get()));

    if (show_debug)
    {
        debug_fps.render();
        debug_window_size.render();
    }
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
    glfwSetCursorPos(window, static_cast<double>(Settings::get_window_width() / 2.0), static_cast<double>(Settings::get_window_height() / 2.0));
}

void Game_State::deactivate(GLFWwindow*)
{

}

void Game_State::key_callback(int key, int, int action)
{
    if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
    {
        show_debug = !show_debug;
    }
}