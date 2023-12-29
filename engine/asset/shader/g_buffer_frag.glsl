#version 460 core
layout(location = 0) out vec3 position;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec4 albedo_spec;

in vec3 v_Position;
in vec2 v_Texcoord;
in vec4 v_Normal;

uniform sampler2D base_color;
void main()
{
    position = v_Position;
    normal = normalize(v_Normal);
    albedo_spec.rgb = texture(base_color, v_Texcoord * 5.0).rgb;
    albedo_spec.a = 0.5;
}
