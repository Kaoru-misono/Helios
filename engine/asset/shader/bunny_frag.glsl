#version 460 core
layout(location = 0) out vec4 FragColor;

in vec3 v_Position;
in vec3 v_Normal;
in vec3 v_Color;

uniform vec3 camera_pos;
uniform samplerCube skybox;

void main()
{
// 	vec3 I = normalize(v_Position - camera_pos);
//  vec3 R = reflect(I, normalize(v_Normal));
// 	vec3 light_color = texture(skybox, R).rgb;
	float ratio = 1.00 / 1.52;
    vec3 I = normalize(v_Position - camera_pos);
    vec3 R = refract(I, normalize(v_Normal), ratio);
	vec3 light_color = texture(skybox, R).rgb;
// 	vec3 light_pos = vec3(0.0, 5.0, 5.0);
// 	float ambient_strength = 0.1;
// 	vec3 ambient = ambient_strength * light_color;
//
// 	vec3 light_dir = normalize(light_pos - v_Position);
// 	vec3 diffuse_color = light_color * max(0.0, dot(v_Normal, light_dir));
// 	vec3 color = ambient + diffuse_color;

	FragColor = vec4(light_color, 1.0);
	//FragColor = texture(texture_samper, v_Texcoord);
}
