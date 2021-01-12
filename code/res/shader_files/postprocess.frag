#version 420 core

in vec2 out_tex_coords;

layout(binding = 0) uniform sampler2D main_pass;
layout(binding = 1) uniform sampler2D god_ray_pass;
uniform vec2 sun_pos;

// Godray parameters 
const float exposure = 0.05f;
const float decay = 0.99;
const float density  = 0.9;
const float weight  = 1.5;
const int NUM_SAMPLES = 170;

out vec4 out_color;

vec4 create_godrays();

void main()
{
    out_color = texture(main_pass, out_tex_coords) + create_godrays();
}

vec4 create_godrays()
{
    // Calculate vector from pixel to light source in screen space.    
    vec2 delta_tex_coord = vec2(out_tex_coords - sun_pos);
    delta_tex_coord *= 1.0 / float(NUM_SAMPLES) * density;

    vec2 tex_coord = out_tex_coords;

    float illumination_decay = 1.0;
    vec4 godray_color = vec4(0,0,0,0);

    for(int i = 0; i < NUM_SAMPLES; i++)
	{
        // Step sample location along ray.
		tex_coord -= delta_tex_coord;

        // Retrieve sample at new location.
		vec4 s = texture(god_ray_pass, tex_coord);

        // Apply sample attenuation scale/decay factors.
		s *= illumination_decay * weight;

		godray_color += s;

        // Update exponential decay factor.
		illumination_decay *= decay;
	}

    return godray_color * exposure;
}