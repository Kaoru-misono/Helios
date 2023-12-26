#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_Texcoord;


out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_Texcoord;

uniform mat4 model_matrix;
layout (std140, binding = 0) uniform transforms
{
	mat4 view_matrix;
	mat4 projection_matrix;
};

void main()
{
	v_Position = vec3(model_matrix * vec4(a_Position, 1.0));
	v_Normal = mat3(transpose(inverse(model_matrix))) * a_Normal;
	v_Texcoord = a_Texcoord;
	gl_Position = projection_matrix * view_matrix * vec4(v_Position, 1.0);
}
