#pragma once
#include <glad/glad.h>
#include "rhi/buffer.hpp"

namespace Helios
{
    class OpenGL_Buffer : public RHI_Buffer
    {
    public:
        OpenGL_Buffer() = default;
        OpenGL_Buffer(GLenum buffer_type, RHI_Usage_Flag flag, uint32_t size, Data_Array data_array);
        OpenGL_Buffer(GLenum buffer_type, RHI_Usage_Flag flag, uint32_t size, std::vector<Data_Array> data_array);
        virtual ~OpenGL_Buffer() override;
        auto create_glbuffer(GLvoid* data) -> void;
        auto create_glbuffer(std::vector<GLvoid*> data) -> void;
        auto bind() -> void override;
    private:
        GLuint resource;
        GLenum type_;
        uint32_t size_;
    };
}
