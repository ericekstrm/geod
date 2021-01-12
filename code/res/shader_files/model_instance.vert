#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coord;
layout (location = 3) in mat4 instance_transform;

out VS_OUT {
	vec3 normal;
	vec2 tex_coord;
	vec3 fragment_position;
	vec4 fragment_position_light_space;
} vs_out;

uniform mat4 world_matrix;
uniform mat4 projection_matrix;
uniform mat4 light_space_matrix;
//uniform mat4 instance_transforms[200];

void main(void)
{
	vs_out.normal = transpose(inverse(mat3(instance_transform))) * in_normal;
	vs_out.tex_coord = in_tex_coord;
	vs_out.fragment_position = vec3(instance_transform * vec4(in_position, 1.0));
	vs_out.fragment_position_light_space = light_space_matrix * vec4(vs_out.fragment_position, 1);
	gl_Position = projection_matrix * world_matrix * instance_transform * vec4(in_position, 1.0);
}
