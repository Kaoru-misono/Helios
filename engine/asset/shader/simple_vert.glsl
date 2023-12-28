#version 460 core
layout(location = 0) in vec3 a_Position;


out vec3 v_Position;
out vec4 v_Fragpos_light_space;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 light_matrix;


void main()
{
	v_Position = vec3(model_matrix * vec4(a_Position, 1.0));
	v_Fragpos_light_space = light_matrix * vec4(v_Position, 1.0);
	gl_Position = projection_matrix * view_matrix * vec4(v_Position, 1.0);
}
