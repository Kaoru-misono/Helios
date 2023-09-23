#include "opengl_buffer.hpp"
#include "logger/logger_marco.hpp"

namespace Helios
{
    OpenGL_Buffer::OpenGL_Buffer(GLenum buffer_type, RHI_Usage_Flag flag, uint32_t size, Data_Array data_array)
        : resource(0), type_(buffer_type), size_(size)
    {
        GLvoid* data_base = data_array.data;
        create_glbuffer(data_base);
    }

    OpenGL_Buffer::OpenGL_Buffer(GLenum buffer_type, RHI_Usage_Flag flag, uint32_t size, std::vector<Data_Array> data_array)
        : resource(0), type_(buffer_type), size_(size)
    {
        std::vector<GLvoid*> data_base;
        for (auto& data: data_array) {
            data_base.emplace_back(data.data);
        }
        create_glbuffer(data_base);
    }

    OpenGL_Buffer::~OpenGL_Buffer()
    {
        glDeleteBuffers(1, &resource);
    }

    auto OpenGL_Buffer::create_glbuffer(GLvoid* data) -> void
    {
        glGenBuffers(1, &resource);
        bind();
        //TODO: STATIC_DRAW be an variable
        glBufferData(type_, size_, data, GL_STATIC_DRAW);
        LOG_TRACE("Buffer create seccess !");
    }

    auto OpenGL_Buffer::create_glbuffer(std::vector<GLvoid*> data) -> void
    {
        glGenBuffers(1, &resource);
        bind();
        //TODO: STATIC_DRAW be an variable
        glBufferData(type_, size_, nullptr, GL_STATIC_DRAW);
        auto stride = 0;
        for (auto sub_data: data) {
            glBufferSubData(GL_ARRAY_BUFFER, stride, sizeof(sub_data), sub_data);
            stride += sizeof(sub_data);
        }
        LOG_TRACE("Buffer create seccess !");
    }

    auto OpenGL_Buffer::bind() -> void
    {
        glBindBuffer(type_, resource);
    }

}