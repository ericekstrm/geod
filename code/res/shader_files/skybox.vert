#version 400 core

layout (location = 0) in vec3 in_position;

out vec3 out_tex_coords;

uniform mat4 projection_matrix;
uniform mat4 world_matrix;

void main()
{
    out_tex_coords = in_position;
    gl_Position = projection_matrix * world_matrix * vec4(in_position, 1.0);
}
