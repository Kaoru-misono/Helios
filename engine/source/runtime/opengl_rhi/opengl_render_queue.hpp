#pragma once
#include "rhi/rhi_defination.hpp" 
#include "opengl_draw_command.hpp"
#include <vector>
namespace Helios
{
    using namespace rhi;
    struct OpenGL_Render_Queue final : RHI_Render_Queqe
    {
        std::vector<OpenGL_Draw_Command> draw_commands;
    };
    
}