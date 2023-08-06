#pragma once
#include "rhi/rhi_defination.hpp"
#include "opengl_render_queue.hpp"
#include "opengl_gpu_program.hpp"
#include <memory>

namespace Helios
{
    using namespace rhi;
    
    struct OpenGL_Pass final : RHI_Pass
    {
        
        OpenGL_Render_Queue queue;
        std::unique_ptr<OpenGL_GPU_Program> gpu_program;
    };
    
}