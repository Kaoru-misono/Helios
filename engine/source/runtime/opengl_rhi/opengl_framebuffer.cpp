#include "opengl_framebuffer.hpp"
#include "logger/logger_marco.hpp"
namespace Helios
{
    OpenGL_Framebuffer::OpenGL_Framebuffer(glm::vec2 buffer_size)
    {
		glGenFramebuffers(1, &resource);
		glBindFramebuffer(GL_FRAMEBUFFER, resource);

		glGenTextures(1, &texColorBuffer);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buffer_size.x, buffer_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, buffer_size.x, buffer_size.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    		LOG_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    OpenGL_Framebuffer::~OpenGL_Framebuffer()
    {
		glDeleteFramebuffers(1, &resource);
    }

	auto OpenGL_Framebuffer::bind() -> void
    {
		glBindFramebuffer(GL_FRAMEBUFFER, resource);
    }

	auto OpenGL_Framebuffer::unbind() -> void
    {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
