#version 420 core

in vec2 out_tex_coords;

layout(binding = 0) uniform sampler2D tex;
uniform vec3 font_color;

out vec4 out_color;

void main()
{
	out_color = vec4(font_color, 1) * texture(tex, out_tex_coords);
}
