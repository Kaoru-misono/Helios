# pragma once
#include <string>
#include <stdint.h>
#include <array>
#include <memory>
#include <vector>
#include <optional>

namespace Helios
{
    struct Data_Array;
    struct RHI_Buffer;

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

    struct Vertex_Array_Specifier
    {
        Vertex_Array_Specifier(const std::initializer_list<Vertex_Attribute_Descriptor>& attributes):attributes_(attributes)
        {
            for(auto& attribute : attributes_)
            {
                attribute.offset = stride;
                stride += attribute.size;
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
}