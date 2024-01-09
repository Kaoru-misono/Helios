#version 460 core
layout(location = 0) out vec4 position_depth;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec4 albedo_spec;

in vec3 v_Position;
in vec2 v_Texcoord;
in vec3 v_Normal;

const float NEAR = 0.1; // 投影矩阵的近平面
const float FAR = 1000.0f; // 投影矩阵的远平面
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // 回到NDC
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}

uniform sampler2D base_color;
void main()
{
    position_depth = vec4(v_Position, LinearizeDepth(gl_FragCoord.z));
    normal = normalize(v_Normal);
    albedo_spec.rgb = texture(base_color, v_Texcoord).rgb;
    albedo_spec.a = 0.5;
}
