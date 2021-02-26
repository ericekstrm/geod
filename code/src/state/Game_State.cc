#include "Game_State.h"

#include "Flying_Camera.h"
#include "Settings.h"

#include <iostream>

Game_State::Game_State()
{
    srand(time(0));

    camera = std::make_unique<Flying_Camera>(vec3{20, 10, 20}, vec3{-1, 0, -1});

    lights.add_pos_light(vec3{35,6,5}, vec3 {1,1,1});

    std::cout << water.get_reflection_framebuffer().get_texture_id() << std::endl;
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
    shadowmap.clear();
    //renderer.render_to_shadowmap(shadowmap, {&terrain});
    renderer.render_to_shadowmap(shadowmap, scene);

    // ===| render water reflection and refraction |===

    glEnable(GL_CLIP_DISTANCE0);
    water.bind_refraction_framebuffer();
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer.render(camera.get(), lights, shadowmap, scene, vec4{0, -1, 0, water.get_height()});

    vec3 new_camera_pos {camera->get_position()};
    new_camera_pos.y -= 2 * (camera->get_position().y - water.get_height());
    vec3 new_camera_dir {camera->get_direction()};
    new_camera_dir.y *= -1;
    std::unique_ptr<Camera> flipped_camera {std::make_unique<Flying_Camera>(new_camera_pos, new_camera_dir)};

    water.bind_reflection_framebuffer();
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer.render_skybox(skybox, camera.get());
    renderer.render(flipped_camera.get(), lights, shadowmap, scene, vec4{0, 1, 0, -water.get_height()});
    water.unbind_framebuffer();
    glDisable(GL_CLIP_DISTANCE0);


    // ===| Main render |===

    main_fbo.bind();
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.render_skybox(skybox, camera.get());

    if (debug_render_as_wireframe) 
    {
    renderer.render_wireframe(camera.get(), lights, shadowmap, scene);
    } else
    {
        renderer.render(camera.get(), lights, shadowmap, scene);
    }

    renderer.render_water(camera.get(), lights, shadowmap, water);

    main_fbo.unbind();

    sun_framebuffer.clear();
    renderer.render_sun(sun_framebuffer, lights, camera.get());
    //renderer.render_godray(sun_framebuffer, camera.get(), {&terrain});
    //TODO: framebuffer cleared between render calls here. FIX!
    renderer.render_godray(sun_framebuffer, camera.get(), scene);


    // ===| render to screen |===

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    main_image.render(lights.get_sun_screen_position(camera.get()));

    reflection_image.render();
    refraction_image.render();

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
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        debug_render_as_wireframe = ! debug_render_as_wireframe;
    }
}