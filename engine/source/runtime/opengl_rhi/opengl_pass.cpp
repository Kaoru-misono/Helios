#include "opengl_pass.hpp"
#include "logger/logger_marco.hpp"

namespace Helios
{
    OpenGL_Pass::OpenGL_Pass(std::shared_ptr<OpenGL_Shader>& vert_shader, std::shared_ptr<OpenGL_Shader>& frag_shader)
    {
        if(vert_shader != nullptr)
        vertex_shader = vert_shader;

        if(frag_shader != nullptr)
        fragment_shader = frag_shader;

        gpu_program = std::make_unique<OpenGL_GPU_Program>();
    }

    auto OpenGL_Pass::shader_process() -> void
    {
        if(vertex_shader == nullptr || fragment_shader == nullptr)
        {
            LOG_INFO("[ERROR]: You need to set vertex shader or fragment shader!");
            return;
        }
        //gpu_program->bind();
        gpu_program->add_vertex_shader(vertex_shader);
        gpu_program->add_fragment_shader(fragment_shader);
        gpu_program->link_shader();
    }

    auto OpenGL_Pass::update() -> void
    {
        for(auto uniform: uniforms)
        {
            gpu_program->set_uniform(uniform.first, uniform.second);
        }
    }

    auto OpenGL_Pass::render() -> void
    {
        for(auto& cmd: draw_commands)
        {
            glDrawArrays(GL_TRIANGLES, 0, 3000);
        }
    }
}