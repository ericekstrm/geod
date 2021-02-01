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

private:

    Model_Shader shader {};
    std::vector<Model> models {};
    Model m {"rock1"};
    Terrain terrain {};

    std::unique_ptr<Camera> camera;

    Skybox skybox {};

    Light_Container lights {};

    Road road {};

    //god ray stuff
    Framebuffer sun_framebuffer {};

    Shadowmap shadowmap {lights.get_sun_position()};

    Framebuffer main_fbo {};
    Main_Image main_image {main_fbo.get_texture_id(), sun_framebuffer.get_texture_id()};

    Renderer renderer {};
};
