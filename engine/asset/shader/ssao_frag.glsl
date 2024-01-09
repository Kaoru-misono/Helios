#version 460 core
layout(location = 0) out float FragColor;
in vec2 v_TexCoords;

uniform sampler2D position_depth;
uniform sampler2D normal;
uniform sampler2D noise_tex;

uniform vec3 samples[64];

// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
int kernelSize = 64;
float radius = 10.0;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1200.0f/4.0f, 900.0f/4.0f);

uniform mat4 projection;

void main()
{
    // Get input for SSAO algorithm
    vec3 fragPos = texture(position_depth, v_TexCoords).xyz;
    vec3 normal = texture(normal, v_TexCoords).rgb;
    vec3 randomVec = texture(noise_tex, v_TexCoords * noiseScale).xyz;
    // Create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // Iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 sample_result = TBN * samples[i]; // From tangent to view-space
        sample_result = fragPos + sample_result * radius;

        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(sample_result, 1.0);
        offset = projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

        // get sample depth
        float sampleDepth = -texture(position_depth, offset.xy).w; // Get depth value of kernel sample

        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth ));
        occlusion += (sampleDepth >= sample_result.z ? 1.0 : 0.0) * rangeCheck;
    }
    occlusion = 1.0 - (occlusion / kernelSize);

    FragColor = occlusion;
}
