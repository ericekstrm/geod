#pragma once

#include "GL/gl.h"

#include "Image.h"

class Main_Image : public Image
{
public:

    Main_Image(unsigned main_pass_id, unsigned god_ray_pass_id);

    void render(vec2 const& sun_pos) const;

private:

    unsigned main_pass_id;
    unsigned god_ray_pass_id;

    Shader post_shader {"image2d.vert", "postprocess.frag"};

};