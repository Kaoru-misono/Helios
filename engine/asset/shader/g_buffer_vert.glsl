#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Texcoord;
layout(location = 2) in vec3 a_Normal;


out vec3 v_Position;
out vec2 v_Texcoord;
out vec4 v_Normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;


void main()
{
	v_Position = vec3(model_matrix * vec4(a_Position, 1.0));
	v_Texcoord = a_Texcoord;
	v_Normal = mat3(transpose(inverse(mat3(model_matrix)))) * a_Normal;
	gl_Position = projection_matrix * view_matrix * vec4(v_Position, 1.0);
}
