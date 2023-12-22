#pragma once
#include "../pre-compile.h"
#include <glm/glm.hpp>
namespace Helios
{
    struct Clear_State final
    {
        bool allow_clear{true};
        glm::vec4 clear_color_value{0.0f, 0.0f, 0.0f, 1.0f};
        bool clear_color{true};
        bool clear_depth{true};
        bool clear_stencil{false};
    };
}
