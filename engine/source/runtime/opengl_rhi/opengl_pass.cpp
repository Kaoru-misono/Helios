#include "opengl_pass.hpp"
#include "opengl_rhi.hpp"
#include "adapter.hpp"
#include "logger/logger_marco.hpp"


namespace Helios
{
    auto OpenGL_Pass::shader_process() -> void
    {
        gpu_program->bind();
        gpu_program->add_vertex_shader(std::make_shared<OpenGL_Shader>(vertex_shader));
        gpu_program->add_fragment_shader(std::make_shared<OpenGL_Shader>(fragment_shader));
        if (geometry_shader != "")
            gpu_program->add_geometry_shader(std::make_shared<OpenGL_Shader>(geometry_shader));
        gpu_program->link_shader();
    }

    auto OpenGL_Pass::set_uniform(std::string name, std::any uniform) -> void
    {
        uniforms[name] = uniform;
        gpu_program->set_uniform(name, uniform);
    }

    auto OpenGL_Pass::set_sampler(std::string name, Texture_Sampler sampler) -> void
    {
        samplers[name] = sampler;
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
        int unit = 0;
        for (auto& sampler: samplers) {
            sampler.second.sampler_unit = unit;
            gpu_program->set_uniform(sampler.first, unit);
            unit++;
        }
    }

    auto OpenGL_Pass::render() -> void
    {
        for(auto& cmd: queue) {
            cmd.vertex_array->bind();
            
            for (auto uniform: cmd.uniform) {
                std::string const type = uniform.second.type().name();
                if (type == "class std::shared_ptr<struct Helios::Texture>") {
                    auto texture = std::any_cast<std::shared_ptr<Texture>>(uniform.second);
                    auto& sampler = samplers[uniform.first];
                    glActiveTexture(GL_TEXTURE0 + sampler.sampler_unit);
                    texture->set_sampler(sampler);
                }
                else
                    gpu_program->set_uniform(uniform.first, uniform.second);
            }
            auto primitive_count = cmd.vertex_array->primitive_count;
            glDrawArrays(to_gl_enum(cmd.primitive), 0, primitive_count * 3);
            glBindVertexArray(0);
        }
    }
}
