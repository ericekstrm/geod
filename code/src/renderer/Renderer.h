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
    
    void render_sun(Framebuffer const& fbo, Light_Container const& lights, Camera const* camera) const;
    void render_godray(Framebuffer const& fbo, Camera const* camera, std::initializer_list<Model const*> models) const;
    void render_godray(Framebuffer const& fbo, Camera const* camera, Scene const& scene) const;

    void render_skybox(Skybox const& skybox, Camera const* camera) const;

    //PBR temp
    void render_PBR(Camera const* camera, Light_Container const& lights, Shadowmap const& shadowmap, std::initializer_list<Model const*> models) const; 

private:

    Shader shader {"model.vert", "model.frag"};

    Shader shadowmap_shader {"shadow.vert", "shadow.frag"};

    Shader god_ray_shader {"model.vert", "godray/darkpass_solid.frag"};

    Shader skybox_shader {"skybox.vert", "skybox.frag"};

    Shader pbr_shader {"pbr.vert", "pbr.frag"};

    Shader light_shader {"color_point.vert", "color_point.frag"};
    Shader sun_shader {"billboard.vert", "godray/sun.frag"};
};
