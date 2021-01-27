#pragma once

#include "GL/gl.h"

#include "State.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Terrain.h"
#include "Terrain_Container.h"
#include "Skybox.h"
#include "Light.h"
#include "Tree.h"
#include "Shadowmap.h"
#include "Image.h"
#include "Framebuffer.h"
#include "Main_Image.h"
#include "Road.h"

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

    void render_scene() const;
    void render_godray_scene() const;
    void render_to_shadowmap() const;

    Model_Shader shader {};
    std::vector<Model> models {};
    Terrain terrain {};

    std::unique_ptr<Camera> camera;

    Skybox_Shader skybox_shader {};
    Skybox skybox {};

    Light_Container lights {};

    Road road {};

    //god ray stuff
    Framebuffer sun_framebuffer {};
    Model_Shader god_ray_shader {"model.vert", "godray/darkpass_solid.frag"};

    Shadowmap shadowmap {lights.get_sun_position()};

    Framebuffer main_fbo {};
    Main_Image main_image {main_fbo.get_texture_id(), sun_framebuffer.get_texture_id()};
};
