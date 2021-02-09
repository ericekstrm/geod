#pragma once

#include "Camera.h"
#include "Light.h"
#include "Shadowmap.h"
#include "Model.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "Skybox.h"
#include "Scene.h"

class Renderer
{
public:

    void render(Camera const* camera, Light_Container const& lights, Shadowmap const& shadowmap, std::initializer_list<Model const*> models) const;
    void render(Camera const* camera, Light_Container const& lights, Shadowmap const& shadowmap, Scene const& scene) const;
    void render_wireframe(Camera const* camera, Light_Container const& lights, Shadowmap const& shadowmap, std::initializer_list<Model const*> models) const;
    void render_wireframe(Camera const* camera, Light_Container const& lights, Shadowmap const& shadowmap, Scene const& scene) const;

    void render_to_shadowmap(Shadowmap shadowmap, std::initializer_list<Model const*> models) const;
    void render_to_shadowmap(Shadowmap shadowmap, Scene const& scene) const;
    
    void render_godray(Framebuffer const& fbo, Light_Container const& lights, Camera const* camera, std::initializer_list<Model const*> models) const;
    void render_godray(Framebuffer const& fbo, Light_Container const& lights, Camera const* camera, Scene const& scene) const;

    void render_skybox(Skybox const& skybox, Camera const* camera) const;

    //PBR temp
    void render_PBR(Camera const* camera, Light_Container const& lights, Shadowmap const& shadowmap, std::initializer_list<Model const*> models) const; 

private:

    Model_Shader shader {};

    Model_Shader shadowmap_shader {"shadow.vert", "shadow.frag"};

    Model_Shader god_ray_shader {"model.vert", "godray/darkpass_solid.frag"};

    Skybox_Shader skybox_shader {};

    //PBR temp
    Model_Shader pbr_shader {"model.vert", "pbr.frag"};
};
