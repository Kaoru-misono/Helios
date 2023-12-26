#include "opengl_texture.hpp"
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

    auto OpenGL_Texture::load_2D_texture(std::string const& path, bool flip) -> std::shared_ptr<OpenGL_Texture>
    {
        auto texture = std::make_shared<OpenGL_Texture>();
        glBindTexture(GL_TEXTURE_2D, texture->texture_id());
        auto image = Image::load(path, flip);
        texture->images.emplace_back(image);
        texture->kind = Texture::Kind::TEX_2D;
        auto channel = image->channel();
        auto format = channel == 1 ? GL_RED : channel == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, image->width(), image->height(), 0, format, GL_UNSIGNED_BYTE, image->data());
        //TODO: remove outside this func
        //glGenerateMipmap(GL_TEXTURE_2D);
        return texture;
    }
}
