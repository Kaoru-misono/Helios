#include "opengl_buffer.hpp"
#include "logger/logger_marco.hpp"

namespace Helios
{
    OpenGL_Buffer::OpenGL_Buffer(GLenum buffer_type, RHI_Usage_Flag flag, uint32_t size, std::shared_ptr<Data_Array> data_array)
    : resource(0), type_(buffer_type), size_(size)
    {
        GLvoid* data_base = data_array->data;
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

    auto OpenGL_Buffer::bind() -> void
    {
        glBindBuffer(type_, resource);
    }

}