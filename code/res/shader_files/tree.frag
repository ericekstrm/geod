#version 400 core

in vec2 out_tex_coord;

uniform sampler2D kd_texture;

out vec4 out_color;

void main()
{
	out_color = texture(kd_texture, out_tex_coord);
}