#version 460 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_Texcoord;

out vec2 v_Texcoord;

void main()
{
    v_Texcoord = a_Texcoord;
    gl_Position = vec4(a_Position, 1.0);
}
