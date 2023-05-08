#version 460 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec3 a_Color;


out vec2 v_Position;
out vec3 v_Color;


void main()
{
	v_Position = a_Position;
	v_Color = a_Color;
	gl_Position = vec4(a_Position, 0.0, 1.0);
}