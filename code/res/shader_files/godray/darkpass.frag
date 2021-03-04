#version 420 core

in VS_OUT {
	vec3 normal;
	vec2 tex_coord;
	vec3 fragment_position;
	vec4 fragment_position_light_space;
} fs_in;

layout(binding = 5) uniform sampler2D opacity_map;

uniform bool has_opacity_map;

out vec4 out_color;

void main()
{
    if (has_opacity_map && texture(opacity_map, fs_in.tex_coord).r < 0.5)
    {
        discard;
    }
    
    out_color = vec4(0, 0, 0, 1);
}
