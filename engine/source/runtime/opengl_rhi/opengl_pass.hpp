#pragma once
#include "rhi/rhi_defination.hpp"
#include "opengl_render_queue.hpp"
#include "opengl_gpu_program.hpp"
#include "opengl_shader.hpp"
#include "opengl_texture.hpp"
#include <memory>

namespace Helios
{
    using namespace rhi;
    
    struct OpenGL_Pass final : RHI_Pass
    {
        OpenGL_Pass() { gpu_program = std::make_unique<OpenGL_GPU_Program>(); }
        OpenGL_Pass(std::shared_ptr<OpenGL_Shader>& vert_shader, std::shared_ptr<OpenGL_Shader>& frag_shader);
        
        auto shader_process() -> void override;

        auto update() -> void override;

        auto render() -> void override;
    };
    
}