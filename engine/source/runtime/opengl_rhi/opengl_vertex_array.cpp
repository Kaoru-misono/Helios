#include "opengl_vertex_array.hpp"
#include "logger/logger_marco.hpp"
#include <iostream>

namespace Helios
{
    OpenGL_Vertex_Array::OpenGL_Vertex_Array(): buffer_size(0)
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

    auto OpenGL_Vertex_Array::add_attributes(Vertex_Attribute&& attribute) -> void
    {
        buffer_size += attribute.buffer_offset;
        attributes.emplace_back(attribute);
    }

    auto OpenGL_Vertex_Array::create_buffer_and_set_data() -> void
    {
        bind();
        unsigned int buffer;
        glGenBuffers(1, &buffer);
    	glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
        auto offset = 0;
        auto idx = 0;
        for (auto& attribute: attributes) {
            glBufferSubData(GL_ARRAY_BUFFER, offset, attribute.buffer_offset, attribute.buffer);
            glEnableVertexAttribArray(idx);
            //TODO: GL_TRUE and GL_FALSE need normalize to controll
            //TODO: calculate stride size of byte so you needn't times sizeof(xx)
            glVertexAttribPointer(
                idx, 
                attribute.element_size, 
                GL_FLOAT, 
                GL_FALSE, 
                attribute.element_size * sizeof(float), 
                (void*)(offset)
            );
            offset += attribute.buffer_offset;
            idx++;
        }
    }
}