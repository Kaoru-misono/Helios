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
        if(vertex_shader == nullptr || fragment_shader == nullptr) {
            LOG_INFO("[ERROR]:{0}-> You need to set vertex shader or fragment shader!", name);
            return;
        }
        gpu_program->bind();
        gpu_program->add_vertex_shader(vertex_shader);
        gpu_program->add_fragment_shader(fragment_shader);
        if (geometry_shader != nullptr)
            gpu_program->add_geometry_shader(geometry_shader);
        gpu_program->link_shader();
    }

    auto OpenGL_Pass::set_uniform(std::string name, std::any uniform) -> void
    {
        uniforms[name] = uniform;
        gpu_program->set_uniform(name, uniform);
    }

    auto OpenGL_Pass::update() -> void
    {
        if (enable_depth_test) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        if (clear_state.allow_clear) {
            auto const color = clear_state.clear_color_value;
            glClearColor(color.x, color.y, color.z, color.w);
            auto color_buffer_bit = clear_state.clear_color ? GL_COLOR_BUFFER_BIT : GL_FALSE;
            auto depth_buffer_bit = clear_state.clear_depth ? GL_DEPTH_BUFFER_BIT : GL_FALSE;
            auto stencil_buffer_bit = clear_state.clear_stencil ? GL_STENCIL_BUFFER_BIT : GL_FALSE;
            auto result = color_buffer_bit | depth_buffer_bit | stencil_buffer_bit;
            if (result)
                glClear(result);
        }
        gpu_program->bind();

    }

    auto OpenGL_Pass::render() -> void
    {
        for(auto& cmd: queue) {
            cmd.vertex_array->bind();
            auto primitive_count = cmd.vertex_array->primitive_count;
            glDrawArrays(GL_TRIANGLES, 0, primitive_count * 3);
            glBindVertexArray(0);
        }
    }
}
