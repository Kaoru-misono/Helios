#pragma once
#include <glad/glad.h>
#include "rhi/rhi_defination.hpp"
#include "rhi/vertex_array_spcifier.hpp"
#include "opengl_rhi.hpp"

namespace Helios
{
    class OpenGL_Vertex_Array : public RHI_Vertex_Array
    {
    public:
    OpenGL_Vertex_Array();
    virtual ~OpenGL_Vertex_Array() override;
    auto bind() -> void override;
    auto add_attributes(Vertex_Attribute&& attribute) -> void override;
    auto create_buffer_and_set_data() -> void override;
    private:
    GLuint resource;
    unsigned int buffer;
    unsigned int buffer_size;
    };
}
