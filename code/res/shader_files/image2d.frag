#version 420 core

in vec2 out_tex_coords;

layout(binding = 0) uniform sampler2D tex;

out vec4 out_color;

void main()
{
	out_color = texture(tex, out_tex_coords);
}
