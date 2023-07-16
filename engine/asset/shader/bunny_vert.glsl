#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;


out vec3 v_Position;
out vec3 v_Normal;
out vec3 v_Color;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform vec3 random_vec;

void main()
{
	v_Position = a_Position;
	v_Normal = a_Normal;
	v_Color = vec3(normalize(a_Position.x), normalize(a_Position.y), normalize(a_Position.z));
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(a_Position, 1.0);
}