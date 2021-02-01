#pragma once

#include "GL/gl.h"

#include "Vector.h"

class Shadowmap
{
public:
    Shadowmap(vec3 const& light_position);

    void activate() const;
    void deactivate() const;

    unsigned get_texture_id() const { return texture_id; }
    vec3 get_light_position() const { return light_position; }
    void set_light_position(vec3 const& new_pos) { light_position = new_pos; }

private:

    vec3 light_position {};

    int width;
    int height;

    unsigned fbo;
    unsigned texture_id;
};