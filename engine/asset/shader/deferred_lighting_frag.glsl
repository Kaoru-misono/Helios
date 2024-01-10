#version 460 core
out vec4 FragColor;

in vec2 v_Texcoord;

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D albedo_spec;
uniform sampler2D ssao_tex;
uniform int debug_mode;
uniform int open_ssao;
uniform float stair_num;
uniform float ambient;
uniform vec3 camera_pos;
uniform vec3 light_pos;

void main()
{
    vec2 g_buffer_texcoord = v_Texcoord;
    if (debug_mode == 1) {
        g_buffer_texcoord = fract(vec2(v_Texcoord.x * float(2.0), v_Texcoord.y * float(2.0)));

    }
    vec3 world_position = texture(position, g_buffer_texcoord).rgb;
    vec3 world_normal = texture(normal, g_buffer_texcoord).rgb;
    vec3 world_albedo = texture(albedo_spec, g_buffer_texcoord).rgb;
    vec3 ssao_result = vec3(texture(ssao_tex, g_buffer_texcoord).r);
    vec3 depth = vec3(texture(position, g_buffer_texcoord).a);
    vec3 light_dir = normalize(light_pos);
    float half_lambert = (max(0, dot(light_dir, world_normal))) * 0.5 + 0.5;
    float phong_diffuse = max(0.0, dot(light_dir, world_normal));
    float floor_toon = floor(half_lambert * stair_num) * (1 / stair_num);
    vec3 ambient_color = world_albedo * ambient;
    vec3 diffuse_color = world_albedo * floor_toon;
    if (open_ssao == 1) ambient_color *= ssao_result;
    vec3 final_color = ambient_color + diffuse_color;
    vec4 vis[4];
    vis[0] = vec4(final_color, 1.0);
    vis[1] = vec4(world_normal, 1.0);
    vis[2] = vec4(world_albedo, 1.0);
    vis[3] = vec4(ssao_result, 1.0);

    if (debug_mode == 1) {
        int grid_id = int(floor((1.0 - v_Texcoord.y) * float(2.0)) * float(2.0) + floor(v_Texcoord.x * float(2.0)));
        FragColor = vis[grid_id];
    } else
    {

        FragColor = vis[0];
    }

}
