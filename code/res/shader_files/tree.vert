#version 400 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coord;


uniform mat4 projection_matrix;
uniform mat4 world_matrix;
uniform mat4 model_matrix;

out vec2 out_tex_coord;

void main()
{
    out_tex_coord = in_tex_coord;
    gl_Position = projection_matrix * world_matrix * model_matrix * vec4(in_position, 1.0);
}
