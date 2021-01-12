#version 420 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coord;

out VS_OUT {
	vec3 normal;
	vec2 tex_coord;
	vec3 fragment_position;
} vs_out;

uniform mat4 model_matrix;
uniform mat4 world_matrix;
uniform mat4 projection_matrix;

void main(void)
{
	vs_out.normal = transpose(inverse(mat3(model_matrix))) * in_normal;
	vs_out.tex_coord = in_tex_coord;
	vs_out.fragment_position = vec3(model_matrix * vec4(in_position, 1.0));
	gl_Position = projection_matrix * world_matrix * model_matrix * vec4(in_position, 1.0);
}
