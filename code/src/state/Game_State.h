#pragma once

#include "GL/gl.h"

#include "State.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Terrain.h"
#include "Skybox.h"
#include "Light.h"
#include "Tree.h"
#include "Shadowmap.h"
#include "Image.h"
#include "Framebuffer.h"
#include "Main_Image.h"
#include "Road.h"
#include "Renderer.h"
#include "Font.h"
#include "Text.h"
#include "Settings.h"

#include <memory>

class Game_State : public State
{
public:
    Game_State();
    ~Game_State();

    virtual void update(float delta_time);
    virtual void render() const;
    virtual void check_input(GLFWwindow* window);
    virtual std::string name() const { return "game"; }

    void activate(GLFWwindow* window) override;
    void deactivate(GLFWwindow* window) override;

    virtual void key_callback(int key, int scanecode, int action) override;

private:

    Terrain terrain {};
    std::unique_ptr<Camera> camera;
    Skybox skybox {};
    Light_Container lights {};
    Road road {};

    Shadowmap shadowmap {lights.get_sun_position()};

    //god ray stuff
    Framebuffer sun_framebuffer {};
    Framebuffer main_fbo {};
    Main_Image main_image {main_fbo.get_texture_id(), sun_framebuffer.get_texture_id()};

    Renderer renderer {};

    //debug texts
    Font font {"arial", 32};
    bool show_debug {false};
    float fps_accumulator = 0;
    Text debug_fps {"", vec2{-0.99, 0.9}, font};
    Text debug_window_size {"Window Size: " + Settings::get_window_size().to_string(), vec2{-0.99, 0.8}, font};

};
