#version 460 core
layout(location = 0) out vec4 FragColor;
			
in vec3 v_Position;
in vec3 v_Normal;
in vec3 v_Color;

void main()
{

	vec3 light_color = vec3(1.0, 1.0, 1.0);
	vec3 light_pos = vec3(0.0, 5.0, 5.0);
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;

	vec3 light_dir = normalize(light_pos - v_Position);
	vec3 diffuse_color = light_color * max(0.0, dot(v_Normal, light_dir));

	vec3 color = ambient + diffuse_color;
	FragColor = vec4(color, 1.0);
	//FragColor = texture(texture_samper, v_Texcoord);
}