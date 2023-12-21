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

    enum struct Vertex_Attribute_Type: int32_t
    {
        none,
        Float,
        Float2,
        Float3,
        Float4
    };

    struct Vertex_Attribute
    {
        std::string element_name{ "" };
        Vertex_Attribute_Type type{ Vertex_Attribute_Type::none };
        uint32_t offset{ 0 };
        uint32_t size{ 0 };
        std::shared_ptr<RHI_Buffer> buffer_token{ nullptr };
    };

    static auto get_size_from_type(Vertex_Attribute_Type& type) -> uint32_t
    {
        switch(type)
        {
            case Vertex_Attribute_Type::Float:
                return 1;
            case Vertex_Attribute_Type::Float2:
                return 2;
            case Vertex_Attribute_Type::Float3:
                return 3;
            case Vertex_Attribute_Type::Float4:
                return 4;
            default:
                return 0;
        }
        //TODO: INFO_WARRING
        return 0;
    }

    struct Vertex_Array_Specifier
    {
        Vertex_Array_Specifier(const std::initializer_list<Vertex_Attribute>& attributes):attributes_(attributes)
        {
            for(auto& attribute : attributes_)
            {
                attribute.offset = stride;
                stride += get_size_from_type(attribute.type);
                //std::cout << attribute.offset << " stride " << stride << std::endl;
            }
        }

        std::vector<Vertex_Attribute>::const_iterator begin() { return attributes_.begin(); }
        std::vector<Vertex_Attribute>::const_iterator end() { return attributes_.end(); }
        uint32_t stride{ 0 };
        std::vector<Vertex_Attribute> attributes_;
    };

    struct RHI_Vertex_Array
    {
        //TODO: subdata specifier
        virtual ~RHI_Vertex_Array() {}
        virtual auto bind() -> void = 0;
        virtual auto set_attributes(Vertex_Array_Specifier& specifier) -> void = 0;
    };

    enum struct Shader_Type
    {
        VERTEX_SHADER,
        FRAGMENT_SHADER
    };

    struct RHI_Shader : RHI_Resource
    {
        virtual ~RHI_Shader() {}
        virtual auto get_shader_id()const -> unsigned int = 0;
        virtual auto get_shader_source()const -> std::string = 0;
        virtual auto get_shader_type()const -> Shader_Type = 0;
    };

    struct RHI_GPU_Program : RHI_Resource
    {
        virtual ~RHI_GPU_Program() {}
        virtual auto bind() -> void = 0;
        virtual auto add_vertex_shader(const std::shared_ptr<RHI_Shader>& vertex_shader) -> void = 0;
        virtual auto add_fragment_shader(const std::shared_ptr<RHI_Shader>& fragment_shader) -> void = 0;
        virtual auto link_shader() -> void = 0;
        virtual auto set_uniform(const std::string& name, std::any const& value) -> void = 0;

    };

    struct RHI_Texture : RHI_Resource
    {
        virtual ~RHI_Texture() {}
        virtual auto set_texture_sampler(unsigned int texture_unit) -> void = 0;
    };

    enum struct Primitive : uint16_t
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
        Primitive primitive;
        std::shared_ptr<RHI_Vertex_Array> vertex_array;
        //std::shared_ptr<RHI_Buffer> vertex_buffer;
        //std::optional<RHI_Buffer> index_buffer;
        std::unordered_map<std::string, std::any> uniform;
        std::unordered_map<std::string, std::any> sampler;
    };

    struct RHI_Render_Queue
    {
        std::vector<RHI_Draw_Command> draw_commands;
    };

    struct RHI_Framebuffer
    {
        struct Attachment
        {
            std::unique_ptr<RHI_Texture> texture;
        };
        std::vector<Attachment> colors;
        Attachment depth;
    };

    struct RHI_Pass
    {
        virtual ~RHI_Pass() {}
        virtual auto shader_process() -> void = 0;
        virtual auto update() -> void = 0;
        virtual auto render() -> void = 0;
        RHI_Framebuffer frame_buffer;
        std::vector<RHI_Draw_Command> draw_commands;
        std::unordered_map<std::string, std::any> uniforms;
        std::unique_ptr<RHI_GPU_Program> gpu_program;
        std::shared_ptr<RHI_Shader> vertex_shader;
        std::shared_ptr<RHI_Shader> fragment_shader;
        std::string name;
    };
}
