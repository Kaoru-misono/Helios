#pragma once
#include "pre-compile.h"
#include "draw_command.hpp"
namespace Helios
{
    struct Render_Queue
    {
        std::vector<RHI_Draw_Command> draw_commands;
    };
}