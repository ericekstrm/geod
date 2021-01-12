#version 330 core

in vec3 out_tex_coords;

uniform samplerCube cube_map;

out vec4 out_color;

void main()
{
    vec4 final_color = texture(cube_map, out_tex_coords);
	out_color = final_color;
}
