#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Texcoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;


out vec3 v_Position;
out vec2 v_Texcoord;
out vec4 v_Fragpos_light_space;

out VS_OUT {
	vec3 tangent_light_pos;
	vec3 tangent_camera_pos;
	vec3 tangent_vert_pos;
} tangent_space;

uniform vec3 camera_pos;
uniform vec3 light_pos;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 light_matrix;


void main()
{
	v_Position = vec3(model_matrix * vec4(a_Position, 1.0));
	v_Texcoord = a_Texcoord;
	v_Fragpos_light_space = light_matrix * vec4(v_Position, 1.0);
	vec3 T = normalize(vec3(model_matrix * vec4(a_Tangent, 0.0)));
	vec3 N = normalize(vec3(model_matrix * vec4(a_Normal, 0.0)));
	vec3 B = cross(T, N);
	mat3 to_TBN = transpose(mat3(T, B, N));
	tangent_space.tangent_light_pos = to_TBN * light_pos;
	tangent_space.tangent_camera_pos = to_TBN * camera_pos;
	tangent_space.tangent_vert_pos = to_TBN * v_Position;
	gl_Position = projection_matrix * view_matrix * vec4(v_Position, 1.0);
}
