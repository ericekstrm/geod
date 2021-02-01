#pragma once

#include "Camera.h"
#include "Light.h"
#include "Shadowmap.h"
#include "Model.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "Skybox.h"

class Renderer
{
public:

    template <class ...Arg_Types>
    void render(Camera const* camera, Light_Container const& lights, Shadowmap const& shadowmap, Arg_Types... args) const { render(camera, lights, shadowmap, {args...}); }

    template <class ...Arg_Types>
    void render_to_shadowmap(Shadowmap shadowmap, Arg_Types... args) const { render_to_shadowmap(shadowmap, {args...}); }

    template <class ...Arg_Types>
    void render_godray(Framebuffer const& fbo, Light_Container const& lights, Camera const* camera, Arg_Types... args) const { render_godray(fbo, lights, camera, {args...}); }

    void render_skybox(Skybox const& skybox, Camera const* camera) const;

private:

    void render(Camera const* camera, Light_Container const& lights, Shadowmap const& shadowmap, std::initializer_list<Model const*> models) const;

    void render_to_shadowmap(Shadowmap shadowmap, std::initializer_list<Model const*> models) const;
    
    void render_godray(Framebuffer const& fbo, Light_Container const& lights, Camera const* camera, std::initializer_list<Model const*> models) const;

    Model_Shader shader {};

    Model_Shader shadowmap_shader {"shadow.vert", "shadow.frag"};

    Model_Shader god_ray_shader {"model.vert", "godray/darkpass_solid.frag"};

    Skybox_Shader skybox_shader {};
};