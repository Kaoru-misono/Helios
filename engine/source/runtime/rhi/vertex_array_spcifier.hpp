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
        virtual auto add_attributes(Vertex_Array_Specifier& specifier) -> void = 0;
    };
}