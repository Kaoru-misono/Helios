#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 light_matrix;
uniform mat4 model_matrix;

void main()
{
    gl_Position = light_matrix * model_matrix * vec4(aPos, 1.0);
}
