#version 460 core

in VS_OUT {
	vec3 normal;
	vec2 tex_coord;
	vec3 fragment_position;
	vec4 fragment_position_light_space;
    mat3 TBN;
} fs_in;

out vec4 out_color;

// Material
layout(binding = 0) uniform sampler2D albedo_map;
layout(binding = 1) uniform sampler2D normal_map;
layout(binding = 2) uniform sampler2D metallic_map;
layout(binding = 3) uniform sampler2D roughness_map;
layout(binding = 4) uniform sampler2D ao_map;

uniform vec3 camera_pos;
layout(binding = 10) uniform sampler2D shadow_map;

// lights
uniform int number_of_lights;
uniform vec3 light_pos_dir[10];
uniform vec3 light_color[10];
uniform vec3 light_attenuation_params[10];
uniform bool light_type[10];

float PI = 3.141592;

//==========================================

float distribution_GGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float geometry_schlick_GGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = geometry_schlick_GGX(NdotV, roughness);
    float ggx1  = geometry_schlick_GGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

//==========================================

vec3 fresnel_schlick(float cos_theta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cos_theta, 0.0), 5.0);
} 

//==========================================

float calc_shadow()
{
	vec3 normal = normalize(fs_in.normal);
	vec3 light_dir = -normalize(light_pos_dir[3]);
	float bias = max(0.005 * (1.0 - dot(normal, light_dir)), 0.00005);

	// perform perspective divide
    vec3 proj_coords = fs_in.fragment_position_light_space.xyz / fs_in.fragment_position_light_space.w;
    // transform to [0,1] range
    proj_coords = proj_coords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadow_map, proj_coords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = proj_coords.z;
    // check whether current frag pos is in shadow
    
	// get depth of current fragment from light's perspective
    float current_depth = proj_coords.z;

	//float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

	float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r; 
            shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;        
        }
    }
    shadow /= 9.0;

	if(proj_coords.z > 1.0)
        shadow = 0.0;

	return shadow;
}

//======================================

//From: https://www.iquilezles.org/www/articles/texturerepetition/texturerepetition.htm

vec4 hash4( vec2 p ) { return fract(sin(vec4( 1.0+dot(p,vec2(37.0,17.0)), 
                                              2.0+dot(p,vec2(11.0,47.0)),
                                              3.0+dot(p,vec2(41.0,29.0)),
                                              4.0+dot(p,vec2(23.0,31.0))))*103.0); }

vec4 textureNoTile( sampler2D samp, in vec2 uv )
{
    vec2 p = floor( uv );
    vec2 f = fract( uv );
	
    // derivatives (for correct mipmapping)
    vec2 ddx = dFdx( uv );
    vec2 ddy = dFdy( uv );
    
    // voronoi contribution
    vec4 va = vec4( 0.0 );
    float wt = 0.0;
    for( int j=-1; j<=1; j++ )
    for( int i=-1; i<=1; i++ )
    {
        vec2 g = vec2( float(i), float(j) );
        vec4 o = hash4( p + g );
        vec2 r = g - f + o.xy;
        float d = dot(r,r);
        float w = exp(-5.0*d );
        vec4 c = textureGrad( samp, uv + o.zw, ddx, ddy );
        va += w*c;
        wt += w;
    }
	
    // normalization
    return va / wt;
}

//======================================

void main(void)
{

	vec3  albedo    = pow(textureNoTile(albedo_map, fs_in.tex_coord).rgb, vec3(2.2));
    float metallic  = textureNoTile(metallic_map, fs_in.tex_coord).r;
    float roughness = textureNoTile(roughness_map, fs_in.tex_coord).r;
    float ao        = textureNoTile(ao_map, fs_in.tex_coord).r;

    vec3 normal = textureNoTile(normal_map, fs_in.tex_coord).rgb;
    normal = normal * 2.0 - 1.0;   
    normal = normalize(fs_in.TBN * normal);

    vec3 view_dir = normalize(camera_pos - fs_in.fragment_position);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

	// reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < number_of_lights; ++i) 
    {
		vec3 light_dir;
		float attenuation;
		float light_dist = length(light_pos_dir[i] - fs_in.fragment_position);

		// calculate per-light radiance
		if (light_type[i])
		{
			light_dir = normalize(-light_pos_dir[i]);
			attenuation = 1;
		} else
		{
			light_dir = normalize(light_pos_dir[i] - fs_in.fragment_position);

            float attenuation_constant = light_attenuation_params[i].x;
 			float attenuation_linear = light_attenuation_params[i].y;	
 			float attenuation_quadratic = light_attenuation_params[i].z;

			float light_dist = length(light_pos_dir[i] - fs_in.fragment_position);
			attenuation = 1.0 / (attenuation_constant + attenuation_linear * light_dist + attenuation_quadratic * light_dist * light_dist);

            //TODO: simpler attenuation based only on distance. Felt to low, but worth revisiting.
			//attenuation = 1.0 ;/// (light_dist * light_dist);
		}

        vec3 halfway_vector = normalize(view_dir + light_dir);

        vec3 radiance = light_color[i] * attenuation;

		// Cook-Torrance BRDF
        float NDF = distribution_GGX(normal, halfway_vector, roughness);
        float G   = geometry_smith(normal, view_dir, light_dir, roughness);
		vec3 F    = fresnel_schlick(max(dot(halfway_vector, view_dir), 0.0), F0);

		vec3 nominator    = NDF * G * F;
        float denominator = 4 * max(dot(normal, view_dir), 0.0) * max(dot(normal, light_dir), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;

		// kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        float shadow  = 0;
        
        if (light_type[i])
        {
            shadow = calc_shadow();
        } 

		Lo += (1 - shadow) * (kD * albedo / PI + specular) * radiance * max(dot(normal, light_dir), 0.0);

	}

	// ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    out_color = vec4(normalize(fs_in.normal), 1.0);
    //out_color = vec4(color, 1.0);
}
