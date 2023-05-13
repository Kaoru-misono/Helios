#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_TexCoord;


out vec3 v_Position;
out vec3 v_Color;
out vec2 v_Texcoord;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	v_Position = a_Position;
	v_Color = a_Color;
	v_Texcoord = a_TexCoord;
	gl_Position = projection_matrix * view_matrix * vec4(a_Position, 1.0);
}