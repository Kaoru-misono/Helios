#version 460 core
out vec4 FragColor;

in vec2 v_Texcoord;

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D albedo_spec;
uniform int debug_mode;

void main()
{
    vec2 g_buffer_texcoord = v_Texcoord;
    if (debug_mode == 1) {
        g_buffer_texcoord = fract(vec2(v_Texcoord.x * float(2.0), v_Texcoord.y * float(2.0)));
        
    }
    vec3 world_position = texture(position, g_buffer_texcoord).rgb;
    vec3 world_normal = texture(normal, g_buffer_texcoord).rgb;
    vec3 world_albedo = texture(albedo_spec, g_buffer_texcoord).rgb;
    vec3 specular = vec3(texture(albedo_spec, g_buffer_texcoord).a);
    vec4 vis[4];
    vis[0] = vec4(world_position, 1.0);
    vis[1] = vec4(world_normal, 1.0);
    vis[2] = vec4(world_albedo, 1.0);
    vis[3] = vec4(specular, 1.0);
    if (debug_mode == 1) {
        int grid_id = int(floor((1.0 - v_Texcoord.y) * float(2.0)) * float(2.0) + floor(v_Texcoord.x * float(2.0)));
        FragColor = vis[grid_id];
    } else
    {
        FragColor = vis[2];
    }
    
}
