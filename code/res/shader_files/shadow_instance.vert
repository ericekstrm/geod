#version 400 core

layout (location = 0) in vec3 in_position;
layout (location = 3) in mat4 instance_transform;

uniform mat4 light_space_matrix;

void main()
{
    gl_Position = light_space_matrix * instance_transform * vec4(in_position, 1.0);
}