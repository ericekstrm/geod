#include "Postprocess_Shader.h"

Postprocess_Shader::Postprocess_Shader()
    : Shader {"image2d.vert", "postprocess.frag"}
{
}

Postprocess_Shader::~Postprocess_Shader()
{
}

void Postprocess_Shader::load_pos_matrix(vec2 const& pos) const
{
    load_mat4("pos_matrix", translation_matrix(pos.x, pos.y, 0));
}

void Postprocess_Shader::load_sun_pos(vec2 const& sun_pos) const
{
    load_vec2("sun_pos", sun_pos);
}