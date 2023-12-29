#version 460 core
layout(location = 0) out vec4 FragColor;

in vec3 v_Position;
in vec2 v_Texcoord;
in vec4 v_Fragpos_light_space;

in VS_OUT {
	vec3 tangent_light_pos;
	vec3 tangent_camera_pos;
	vec3 tangent_vert_pos;
} tangent_space;

uniform float shadow_bias;
uniform sampler2D shadow_map;
uniform sampler2D base_color;
uniform sampler2D normal;

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
    vec3 normal = texture(normal, v_Texcoord * 5.0).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 light_dir = normalize(tangent_space.tangent_light_pos - tangent_space.tangent_vert_pos);
    vec3 view_dir = normalize(tangent_space.tangent_camera_pos - tangent_space.tangent_vert_pos);
    vec3 diffuse = texture(base_color, v_Texcoord * 5.0).rgb * max(dot(light_dir, normal), 0.0);
    vec3 specular = vec3(0.2) * pow(max(dot(normal, normalize(light_dir + view_dir)), 0.0), 32.0);
	FragColor = vec4(diffuse + specular, 1.0) * (1.0 - shadow);
}
