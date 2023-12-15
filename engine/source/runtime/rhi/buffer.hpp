#pragma once
#include "pre-compile.h"
namespace Helios
{
    struct RHI_Buffer
    {
        virtual ~RHI_Buffer() {}
        virtual auto bind() -> void = 0;
    };

    struct Data_Array
    {
        Data_Array(size_t in_size, void* array): size(in_size), data(array) {}
        size_t size{ 0 };
        void* data{ nullptr };
    };

    struct RHI_Buffer_Create_info
    {
        std::vector<Data_Array> data_array;
    };

    enum struct RHI_Usage_Flag : uint32_t
    {
        vertex_buffer,
        index_buffer
    };
}
