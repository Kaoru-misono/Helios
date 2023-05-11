#include "opengl_texture.hpp"
#include "logger/logger_marco.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Helios
{
    OpenGL_Texture::OpenGL_Texture(const std::string& path)
    {
        resource = load_texture(path, format);
    }

    OpenGL_Texture::~OpenGL_Texture()
    {
        glDeleteTextures(1, &resource);
    }

    auto OpenGL_Texture::set_texture_unit(unsigned int texture_unit) -> void
    {
        texture_unit_ = texture_unit;
        glBindTextureUnit(texture_unit, resource);
    }

    auto OpenGL_Texture::load_texture(const std::string& path, GLenum format) -> GLuint
    {
        GLuint textureID;
        glGenTextures(1, &textureID);
        
        stbi_set_flip_vertically_on_load(1);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

        if (data)
        {
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

            //TODO: remove outside this func
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

    auto OpenGL_Texture::get_texture_unit() -> unsigned int 
    {
        return texture_unit_;
    }
}