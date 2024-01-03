#include "opengl_framebuffer.hpp"
#include "opengl_texture.hpp"
#include "logger/logger_marco.hpp"
#include "window/window.hpp"
namespace Helios
{
    OpenGL_Framebuffer::OpenGL_Framebuffer()
    {
		glGenFramebuffers(1, &resource);
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
		// TODO: Hack, resove this
		glViewport(0, 0, Window::instance().get_width(), Window::instance().get_height());
    }

	auto OpenGL_Framebuffer::attach() -> void
	{
		bind();
		const int buffer_size = colors.size();
		int idx = 0;
		for (auto& color: colors) {
			auto texture = std::reinterpret_pointer_cast<OpenGL_Texture>(color.texture);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_TEXTURE_2D, texture->id(), 0);

			idx++;
		}
		GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);
		auto depth_texture = std::reinterpret_pointer_cast<OpenGL_Texture>(depth.texture);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture->id(), 0);

		if (colors.size() == 0) {
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    		LOG_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		unbind();
	}

	auto OpenGL_Framebuffer::blit() -> void
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, resource);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Write to default framebuffer
        // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
        // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
        // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
        glBlitFramebuffer(0, 0, Window::instance().get_width(), Window::instance().get_height(), 0, 0, Window::instance().get_width(), Window::instance().get_height(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
