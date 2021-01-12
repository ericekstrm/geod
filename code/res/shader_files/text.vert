#version 400 core

layout (location = 0) in vec3 in_position;
layout (location = 2) in vec2 in_tex_coords;

uniform mat4 text_pos_matrix;
uniform mat4 char_pos_matrix;

out vec2 out_tex_coords;

void main()
{
    out_tex_coords = in_tex_coords;
    gl_Position = char_pos_matrix * text_pos_matrix * vec4(in_position, 1.0);
}
