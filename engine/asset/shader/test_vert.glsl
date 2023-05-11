#version 460 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_TexCoord;


out vec2 v_Position;
out vec3 v_Color;
out vec2 v_Texcoord;

void main()
{
	v_Position = a_Position;
	v_Color = a_Color;
	v_Texcoord = a_TexCoord;
	gl_Position = vec4(a_Position, 0.0, 1.0);
}