#pragma once

#if (defined (_WIN32) || defined (_WIN64))
    #include "glad/glad.h"
#else
    #include "GL/gl.h"
#endif

#include "Image.h"
#include "Postprocess_Shader.h"

class Main_Image : public Image
{
public:

    Main_Image(unsigned main_pass_id, unsigned god_ray_pass_id);

    void render(vec2 const& sun_pos) const;

private:

    unsigned main_pass_id;
    unsigned god_ray_pass_id;

    Postprocess_Shader post_shader {};

};