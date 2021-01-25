#pragma once

#include "GL/gl.h"

#include "Tree.h"
#include "Shader.h"
#include "Model_Shader.h"
#include "Terrain.h"
#include "Terrain_Container.h"

class Shadowmap
{
public:
    Shadowmap(vec3 const& light_position);

    void activate() const;
    void deactivate() const;

    void render(Tree const& tree) const;
    void render(Terrain_Container const& terrain) const;
    void render(Model const& model) const;

    unsigned get_texture_id() const { return depth_map; }
    vec3 get_position() const { return light_position; }
    void set_light_position(vec3 const& new_pos) { light_position = new_pos; }

private:

    static const int shadow_width = 2048;
    static const int shadow_height = 2048;

    unsigned depth_map_fbo {0};
    unsigned depth_map {0};

    vec3 light_position {};

    Model_Shader shader {"shadow.vert", "shadow.frag"};
    Model_Instance_Shader instance_shader {"shadow_instance.vert", "shadow.frag"};
};