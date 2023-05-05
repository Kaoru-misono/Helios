# pragma once
#include <string>
#include <stdint.h>
#include <array>
#include <memory>

namespace Helios
{
    struct Data_Array;

    enum struct Vertex_Element_Type: int
    {
        none,
        Float,
        Float2,
        Float3,
        Float4
    };

    struct Vertex_Element
    {
        std::string element_name;
        Vertex_Element_Type type;
        uint32_t offset;
        uint32_t stride;
    };

    struct RHI_Buffer
    {

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
        size_t size{ 0 };
        void* data{ nullptr };
    };
}