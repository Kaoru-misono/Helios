# pragma once
#include <string>
#include <stdint.h>
#include <array>
#include <unordered_map>
#include <memory>
#include <vector>
#include <optional>
#include <iostream>
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

    struct Vertex_Attribute_Descriptor
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
        Vertex_Array_Specifier(const std::initializer_list<Vertex_Attribute_Descriptor>& attributes):attributes_(attributes)
        {
            for(auto& attribute : attributes_)
            {
                attribute.offset = stride;
                stride += get_size_from_type(attribute.type);
                //std::cout << attribute.offset << " stride " << stride << std::endl;
            }
        }
        
        std::vector<Vertex_Attribute_Descriptor>::const_iterator begin() { return attributes_.begin(); }
        std::vector<Vertex_Attribute_Descriptor>::const_iterator end() { return attributes_.end(); }
        uint32_t stride{ 0 };
        std::vector<Vertex_Attribute_Descriptor> attributes_;
    };

    struct RHI_Vertex_Array
    {
        virtual ~RHI_Vertex_Array() {}
        virtual auto bind() -> void = 0;
        virtual auto set_attributes(Vertex_Array_Specifier& specifier) -> void = 0;  
    };

    struct RHI_Buffer
    {
        virtual ~RHI_Buffer() {}
        virtual auto bind() -> void = 0;
    };

    struct RHI_Buffer_Create_info
    {
        std::shared_ptr<Data_Array> data_array;
    };

    enum struct RHI_Usage_Flag : uint32_t
    {
        vertex_buffer,
        index_buffer
    };

    struct Data_Array
    {
        Data_Array(size_t in_size, void* array): size(in_size), data(array) {}
        size_t size{ 0 };
        void* data{ nullptr };
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
        virtual auto set_uniform(const std::string& name, const int        value ) -> void = 0;
        virtual auto set_uniform(const std::string& name, const float      value ) -> void = 0;
        virtual auto set_uniform(const std::string& name, const glm::vec2& values) -> void = 0;
        virtual auto set_uniform(const std::string& name, const glm::vec3& values) -> void = 0;
        virtual auto set_uniform(const std::string& name, const glm::vec4& values) -> void = 0;
        virtual auto set_uniform(const std::string& name, const glm::mat3& values) -> void = 0;
        virtual auto set_uniform(const std::string& name, const glm::mat4& values) -> void = 0;
    };

    struct RHI_Texture : RHI_Resource
    {
        virtual ~RHI_Texture() {}
        virtual auto set_texture_unit(unsigned int texture_unit) -> void = 0;
    };
}