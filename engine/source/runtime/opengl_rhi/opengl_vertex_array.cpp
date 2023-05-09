#include "opengl_vertex_array.hpp"
#include "logger/logger_marco.hpp"
#include <iostream>

namespace Helios
{
    OpenGL_Vertex_Array::OpenGL_Vertex_Array()
    {
        glGenVertexArrays(1, &resource);
        bind();
    }

    OpenGL_Vertex_Array::~OpenGL_Vertex_Array()
    {
        glDeleteVertexArrays(1, &resource);
    }

    auto OpenGL_Vertex_Array::bind() -> void
    {
        glBindVertexArray(resource);
    }

    auto  OpenGL_Vertex_Array::set_attributes(Vertex_Array_Specifier& specifier) -> void
    {
        bind();
        auto& attributes = specifier.attributes_;
        int idx = 0;
        for(auto& attribute: attributes)
        {
            glEnableVertexAttribArray(idx);
            //TODO: GL_TRUE and GL_FALSE need normalize to controll
            //TODO: calculate stride size of byte so you needn't times sizeof(xx)
            //std::cout << "idx: " << idx << " size: " << get_size_by_type(attribute.type) << " GLenum: " << get_GLenum_by_type(attribute.type) << " " <<  specifier.stride << " " << attribute.offset << std::endl;
            glVertexAttribPointer(
                idx, 
                get_size_by_type(attribute.type), 
                get_GLenum_by_type(attribute.type), 
                GL_FALSE, 
                specifier.stride * sizeof(float), 
                (void*)(attribute.offset * sizeof(float))
                );
            idx++;
        }
    }

    auto OpenGL_Vertex_Array::get_GLenum_by_type(Vertex_Attribute_Type& type) -> GLenum
    {
        switch(type)
        {
            case Vertex_Attribute_Type::Float:
            case Vertex_Attribute_Type::Float2:
            case Vertex_Attribute_Type::Float3:
                return GL_FLOAT;
            default:
                return 0;
        }

        LOG_ERROR(" Can not get GLenum type by this type !");
        return 0;
    }

    auto OpenGL_Vertex_Array::get_size_by_type(Vertex_Attribute_Type& type) -> GLint
    {
        switch(type)
        {
            case Vertex_Attribute_Type::Float:
                return 1;
            case Vertex_Attribute_Type::Float2:
                return 2;
            case Vertex_Attribute_Type::Float3:
                return 3;
            default:
                return 0;
        }

        LOG_ERROR(" Can not get size by this type !");
        return 0;
    }

}