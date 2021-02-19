#version 400 core

layout (location = 0) in vec3 in_position;
layout (location = 2) in vec2 in_tex_coords;

uniform vec3 position;

out vec2 out_tex_coords;

void main()
{
    out_tex_coords = in_tex_coords;
    gl_Position = vec4(in_position + position, 1.0);
}
