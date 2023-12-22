#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "rhi/rhi_defination.hpp"
namespace Helios
{
    struct OpenGL_Framebuffer final: RHI_Framebuffer
    {
        OpenGL_Framebuffer(glm::vec2 buffer_size);
         ~OpenGL_Framebuffer() override;
        auto bind() -> void override;
        auto unbind() -> void override;
		unsigned int texColorBuffer;
		unsigned int rbo;

        unsigned int resource;
    };
}
