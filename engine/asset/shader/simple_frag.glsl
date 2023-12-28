#version 460 core
layout(location = 0) out vec4 FragColor;

in vec3 v_Position;
in vec4 v_Fragpos_light_space;
uniform vec3 camera_pos;
uniform vec3 light_pos;
uniform float shadow_bias;
uniform sampler2D shadow_map;

void main()
{
	vec3 proj_coords = v_Fragpos_light_space.xyz / v_Fragpos_light_space.w;
    proj_coords = proj_coords * 0.5 + 0.5;
    float cloest_depth = texture(shadow_map, proj_coords.xy).r;
    float current_depth = proj_coords.z;
    //float shadow = (current_depth - shadow_bias) > cloest_depth ? 1.0 : 0.0;
	// PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texelSize).r;
            shadow += (current_depth - shadow_bias) > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
	FragColor = vec4(0.7) * (1.0 - shadow);
}
