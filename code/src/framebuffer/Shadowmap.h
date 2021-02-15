#pragma once

#include "GL/gl.h"

#include "Vector.h"

class Shadowmap
{
public:
    Shadowmap(vec3 const& light_position);

    void clear() const;
    void activate() const;
    void deactivate() const;

    unsigned get_texture_id() const { return texture_id; }
    vec3 get_light_position() const { return light_position; }
    void set_light_position(vec3 const& new_pos) { light_position = new_pos; }

private:

    int width;
    int height;

    vec3 light_position {};

    unsigned fbo;
    unsigned texture_id;
};