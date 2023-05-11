#version 460 core
layout(location = 0) out vec4 FragColor;
			
in vec2 v_Position;
in vec3 v_Color;
in vec2 v_Texcoord;

uniform sampler2D texture_samper;

void main()
{
	//FragColor = vec4(v_Color, 1.0);
	FragColor = texture(texture_samper, v_Texcoord);
}