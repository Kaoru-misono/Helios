#pragma once
#include "rhi/rhi_defination.hpp"
#include "rhi/pass.hpp"
#include "opengl_gpu_program.hpp"
#include "opengl_shader.hpp"
#include "opengl_texture.hpp"
#include <memory>

namespace Helios
{
    struct OpenGL_Pass final : Pass
    {
        OpenGL_Pass(std::string&& name) { this->name = name; gpu_program = std::make_unique<OpenGL_GPU_Program>(); }
        OpenGL_Pass(std::shared_ptr<OpenGL_Shader>& vert_shader, std::shared_ptr<OpenGL_Shader>& frag_shader);

        auto shader_process() -> void override;
        auto set_uniform(std::string name, std::any uniform) -> void override;

        auto update() -> void override;
        auto render() -> void override;
    };

}
