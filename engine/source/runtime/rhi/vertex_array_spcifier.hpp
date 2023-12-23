#pragma once
#include "pre-compile.h"
#include "rhi_defination.hpp"
namespace Helios
{
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
        unsigned int element_size{};
        unsigned int buffer_offset{};
        void* buffer{};
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

    struct RHI_Vertex_Array
    {
        //TODO: subdata specifier
        virtual ~RHI_Vertex_Array() {}
        virtual auto bind() -> void = 0;
        virtual auto add_attributes(Vertex_Attribute&& attribute) -> void = 0;
        virtual auto create_buffer_and_set_data() -> void = 0;
        unsigned int primitive_count{0};
    protected:
        std::vector<Vertex_Attribute> attributes;
    };
}