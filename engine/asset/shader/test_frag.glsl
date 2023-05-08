#version 460 core
layout(location = 0) out vec4 FragColor;
			
in vec2 v_Position;
in vec3 v_Color;


void main()
{
	FragColor = vec4(v_Color, 1.0);
}