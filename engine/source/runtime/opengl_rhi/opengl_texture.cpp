#include "opengl_texture.hpp"
#include "adapter.hpp"
#include "logger/logger_marco.hpp"

namespace Helios
{
    OpenGL_Texture::OpenGL_Texture()
    {
        glGenTextures(1, &resource);
    }

    OpenGL_Texture::~OpenGL_Texture()
    {
        glDeleteTextures(1, &resource);
    }

    auto OpenGL_Texture::bind() const -> void
    {
        glBindTexture(gl_kind, resource);
    }

    auto OpenGL_Texture::unbind() const -> void
    {
        glBindTexture(gl_kind, 0);
    }

    auto OpenGL_Texture::set_sampler(Texture_Sampler const& sampler) const -> void
    {
        bind();
		glTexParameteri(gl_kind, GL_TEXTURE_WRAP_S, to_gl_enum(sampler.warp_s));
		glTexParameteri(gl_kind, GL_TEXTURE_WRAP_T, to_gl_enum(sampler.warp_t));
        if (sampler.warp_r != Texture_Sampler::Warp::none)
		    glTexParameteri(gl_kind, GL_TEXTURE_WRAP_R, to_gl_enum(sampler.warp_r));
		glTexParameteri(gl_kind, GL_TEXTURE_MIN_FILTER, to_gl_enum(sampler.min_filter));
		glTexParameteri(gl_kind, GL_TEXTURE_MAG_FILTER, to_gl_enum(sampler.mag_filter));
    }

    auto OpenGL_Texture::load_2D_texture(std::string const& path, bool flip) -> std::shared_ptr<OpenGL_Texture>
    {
        auto texture = std::make_shared<OpenGL_Texture>();
        texture->kind = Texture::Kind::TEX_2D;
        texture->gl_kind = GL_TEXTURE_2D;
        texture->bind();
        auto image = Image::load(path, flip);
        texture->images.emplace_back(image);
        auto channel = image->channel();
        auto format = channel == 1 ? GL_RED : channel == 3 ? GL_RGB : GL_RGBA;
        texture->format = channel == 1 ? Texture::Format::r8 : channel == 3 ? Texture::Format::rgb8 : Texture::Format::rgba8;
        glTexImage2D(texture->gl_kind, 0, format, image->width(), image->height(), 0, format, GL_UNSIGNED_BYTE, image->data());
		glGenerateMipmap(texture->gl_kind);
        texture->unbind();
        return texture;
    }

    auto OpenGL_Texture::load_cube_map_texture(std::vector<std::string> const& paths, bool flip) -> std::shared_ptr<OpenGL_Texture>
    {
        auto texture = std::make_shared<OpenGL_Texture>();
        texture->gl_kind = GL_TEXTURE_CUBE_MAP;
        texture->bind();
        int face = 0;
        for (auto path: paths) {
            auto image = Image::load(path, flip);
            texture->images.emplace_back(image);
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
				0,
                GL_RGB,
                image->width(),
                image->height(),
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                image->data()
			);
            face++;
        }
        texture->unbind();
        return texture;
    }

    auto OpenGL_Texture::create_texture(Texture::Kind kind, Texture::Format format, int width, int height) -> std::shared_ptr<OpenGL_Texture>
    {
        auto texture = std::make_shared<OpenGL_Texture>();
        texture->kind = kind;
        texture->format = format;
        texture->gl_kind = GL_TEXTURE_2D;
        texture->width = width;
        texture->height = height;
        texture->bind();
        // TODO: 1024 should be changeable
        if (format == Texture::Format::depth24)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        else if (format == Texture::Format::rgba8)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        else if (format == Texture::Format::rgb16f)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        texture->unbind();
        return texture;
    }
}
