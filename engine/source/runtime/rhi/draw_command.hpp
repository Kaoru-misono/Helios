#pragma once
#include "../pre-compile.h"
#include "rhi_defination.hpp"
#include "rhi/vertex_array_spcifier.hpp"
#include "rhi/texture.hpp"
namespace Helios
{
    enum struct Primitive
    {
        none,
        points,
        lines,
        line_strip,
        line_loop,
        trianges,
        triangle_strip,
    };

    struct RHI_Draw_Command
    {
        virtual ~RHI_Draw_Command() {}
        Primitive primitive{Primitive::trianges};
        std::shared_ptr<RHI_Vertex_Array> vertex_array{};
        //std::shared_ptr<RHI_Buffer> vertex_buffer;
        //std::optional<RHI_Buffer> index_buffer;
        // Uniforms changed between cmds should be set here
        std::unordered_map<std::string, std::any> uniform{};
        std::unordered_map<std::string, Texture_Sampler> sampler{};
    };
}
