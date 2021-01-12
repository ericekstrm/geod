#version 400 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coord;

uniform mat4 projection_matrix;
uniform mat4 world_matrix;
uniform vec3 position;

void main()
{
    gl_Position = projection_matrix * world_matrix * vec4(in_position + position, 1.0);
}
