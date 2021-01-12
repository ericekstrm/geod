#pragma once

#if (defined (_WIN32) || defined (_WIN64))
    #include "glad/glad.h"
#else
    #include "GL/gl.h"
#endif

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
    Terrain_Container terrain {};
    std::vector<Tree> trees {};

    std::unique_ptr<Camera> camera;

    Skybox_Shader skybox_shader {};
    Skybox skybox {};

    Light_Container lights {};

    //god ray stuff
    Framebuffer sun_framebuffer {};
    Model_Shader god_ray_shader {"model.vert", "godray/darkpass_solid.frag"};
    Model_Instance_Shader god_ray_leaf_shader {"model_instance.vert", "godray/darkpass_leaf.frag"};

    Shadowmap shadowmap {lights.get_sun_position()};

    Framebuffer main_fbo {};
    Main_Image main_image {main_fbo.get_texture_id(), sun_framebuffer.get_texture_id()};
};
