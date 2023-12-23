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
    auto add_attributes(Vertex_Array_Specifier& specifier) -> void override;

    static auto get_GLenum_by_type(Vertex_Attribute_Type& type) -> GLenum;
    static auto get_size_by_type(Vertex_Attribute_Type& type) -> GLint;

    private:
    GLuint resource;
    };
}
