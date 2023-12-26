#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 v_Normal[];
out vec3 g_Normal;
out vec3 g_Position;
uniform float time;
vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}
vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 0.0);
}
void main() {
    vec3 normal = GetNormal();
    gl_Position = explode(gl_in[0].gl_Position, normal);
    g_Position = vec3(gl_Position);
    g_Normal = v_Normal[0];
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    g_Position = vec3(gl_Position);
    g_Normal = v_Normal[1];
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    g_Position = vec3(gl_Position);
    g_Normal = v_Normal[2];
    EmitVertex();
    EndPrimitive();
}
