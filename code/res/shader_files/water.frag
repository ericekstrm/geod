#version 460 core

in VS_OUT {
	vec3 normal;
	vec2 tex_coord;
	vec3 fragment_position;
	vec4 fragment_position_light_space;
    mat3 TBN;
} fs_in;
in vec4 clip_space;

out vec4 out_color;

// Material
layout(binding = 0) uniform sampler2D albedo_map;
layout(binding = 1) uniform sampler2D normal_map;
layout(binding = 2) uniform sampler2D metallic_map;
layout(binding = 3) uniform sampler2D roughness_map;
layout(binding = 4) uniform sampler2D ao_map;

layout(binding = 11) uniform sampler2D reflection_texture;
layout(binding = 12) uniform sampler2D refraction_texture;

uniform vec3 camera_pos;
layout(binding = 10) uniform sampler2D shadow_map;

// lights
uniform int number_of_lights;
uniform vec3 light_pos_dir[10];
uniform vec3 light_color[10];
uniform vec3 light_attenuation_params[10];
uniform bool light_type[10];

void main(void)
{
    vec2 ndc = (clip_space.xy/clip_space.w)/2 + 0.5; 

    vec2 reflectionTexCoord = vec2(ndc.x, -ndc.y);
    vec2 refractionTexCoord = ndc;

    reflectionTexCoord.x = clamp(reflectionTexCoord.x, 0.001, 0.999);
    reflectionTexCoord.y = clamp(reflectionTexCoord.y, -0.999, -0.001);
    refractionTexCoord = clamp(refractionTexCoord, 0.001, 0.999);

    vec4 reflection = texture(reflection_texture, reflectionTexCoord);
    vec4 refraction = texture(refraction_texture, refractionTexCoord);

    float refractive_factor = dot(normalize(camera_pos - fs_in.fragment_position), vec3(0, 1, 0));

    out_color = mix(reflection, refraction, refractive_factor);

    //out_color = vec4(clip_space);
}