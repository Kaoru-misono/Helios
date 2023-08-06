#pragma once
#include "rhi/rhi_defination.hpp"
#include "opengl_vertex_array.hpp"
#include "opengl_buffer.hpp"
#include <optional>
#include <unordered_map>
#include <string>
#include <any>

namespace Helios
{
    using namespace rhi;

    struct OpenGL_Draw_Command final : RHI_Draw_Command
    {
        OpenGL_Vertex_Array vertex_array;
        OpenGL_Buffer vertex_buffer;
        std::optional<OpenGL_Buffer> index_buffer;
        std::unordered_map<std::string, std::any> uniform;
        std::unordered_map<std::string, std::any> sampler;
    };
}