#pragma once
#include <glad/glad.h>
#include "rhi/rhi_defination.hpp"

namespace Helios
{
    class OpenGL_Buffer : public RHI_Buffer
    {
    public:
        OpenGL_Buffer() = default;
        OpenGL_Buffer(GLenum buffer_type, RHI_Usage_Flag flag, uint32_t size, std::shared_ptr<Data_Array> data_array);
        auto create_glbuffer(GLvoid* data) -> void;
        auto bind() -> void; 
    private:
        GLuint resource;
        GLenum type;
        uint32_t _size;
    };
}