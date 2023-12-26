# pragma once
#include "pre-compile.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace Helios
{
    struct Data_Array;
    struct RHI_Buffer;

    struct RHI_Resource
    {
        virtual ~RHI_Resource() {}
    };

    enum struct Shader_Type
    {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        GEOMETRY_SHADER,
    };

    struct RHI_Shader : RHI_Resource
    {
        virtual ~RHI_Shader() {}
        virtual auto shader_id()const -> unsigned int = 0;
        virtual auto shader_source()const -> std::string = 0;
        virtual auto shader_type()const -> Shader_Type = 0;
    };

    struct RHI_GPU_Program : RHI_Resource
    {
        virtual ~RHI_GPU_Program() {}
        virtual auto bind() -> void = 0;
        virtual auto add_vertex_shader(const std::shared_ptr<RHI_Shader>& vertex_shader) -> void = 0;
        virtual auto add_fragment_shader(const std::shared_ptr<RHI_Shader>& fragment_shader) -> void = 0;
        virtual auto add_geometry_shader(const std::shared_ptr<RHI_Shader>& geometry_shader) -> void = 0;
        virtual auto link_shader() -> void = 0;
        virtual auto set_uniform(const std::string& name, std::any const& value) -> void = 0;
        virtual auto id() -> unsigned int = 0;

    };

    struct RHI_Texture : RHI_Resource
    {
        virtual ~RHI_Texture() {}
        virtual auto set_texture_sampler(unsigned int texture_unit) -> void = 0;
    };

    struct RHI_Framebuffer
    {
        struct Attachment
        {
            std::unique_ptr<RHI_Texture> texture;
        };
        std::vector<Attachment> colors;
        Attachment depth;
        virtual ~RHI_Framebuffer() {}
        virtual auto bind() -> void = 0;
        virtual auto unbind() -> void = 0;
    };


}
