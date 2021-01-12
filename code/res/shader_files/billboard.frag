#version 420 core

in VS_OUT {
	vec3 normal;
	vec2 tex_coord;
	vec3 fragment_position;
} fs_in;

out vec4 out_color;

layout(binding = 0) uniform sampler2D tex;

void main(void)
{
	out_color = texture(tex, fs_in.tex_coord);
}
