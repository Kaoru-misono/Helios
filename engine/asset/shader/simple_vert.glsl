#version 460 core
layout(location = 0) in vec3 a_Position;


out vec3 v_Position;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	v_Position = a_Position;
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(a_Position, 1.0);
}
