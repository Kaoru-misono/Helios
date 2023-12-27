#version 460 core
layout(location = 0) out vec4 FragColor;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_Texcoord;
uniform vec3 camera_pos;
uniform vec3 light_dir;
uniform sampler2D base_color;
uniform int mesh_id;

// from https://www.shadertoy.com/view/XlGcRh

// commonly used constants
#define HASH_FUNCTION_CONSTANT_0 0xcc9e2d51u
#define HASH_FUNCTION_CONSTANT_1 0x1b873593u

// helper Functions
uint rotl(uint x, uint r)
{
    return (x << r) | (x >> (32u - r));
}

uint fmix(uint h)
{
    h ^= h >> 16;
    h *= 0x85ebca6bu;
    h ^= h >> 13;
    h *= 0xc2b2ae35u;
    h ^= h >> 16;
    return h;
}

// Adapted from MurmurHash3_x86_32 from https://github.com/aappleby/smhasher
uint murmur3(uint seed)
{
    uint h = 0u;
    uint k = seed;

    k *= HASH_FUNCTION_CONSTANT_0;
    k = rotl(k,15u);
    k *= HASH_FUNCTION_CONSTANT_1;

    h ^= k;
    h = rotl(h,13u);
    h = h*5u+0xe6546b64u;

    h ^= 4u;

    return fmix(h);
}

vec3 map_uint_to_color(uint index)
{
    uint hash = murmur3(index);

    vec3 color = vec3(
        (hash >>  0) & 255,
        (hash >>  8) & 255,
        (hash >> 16) & 255
    );

    return color * (1.0 / 255.0);
}

void main()
{
    vec3 color = map_uint_to_color(mesh_id);
    color = texture(base_color, v_Texcoord).rgb;
    vec3 diffuse_color = color * max(0.0, dot(v_Normal, light_dir)) * 0.1;
    vec3 view_dir = camera_pos - v_Position;
    vec3 half_dir = normalize(view_dir + light_dir);
    vec3 specular_color = vec3(1.0) * max(0.0, dot(v_Normal, half_dir)) * 0.01;
	FragColor = vec4(color, 1.0);
}
