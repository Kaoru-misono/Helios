#include "opengl_texture.hpp"
#include "logger/logger_marco.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Helios
{
    OpenGL_Texture::OpenGL_Texture(const std::string& path)
    {
        texture_data = load_texture(path);
        create_texture();
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

    auto OpenGL_Texture::load_texture(const std::string& path) -> Texture_Data
    {
        Texture_Data texture_data;
        stbi_set_flip_vertically_on_load(1);

        texture_data.data = stbi_load(path.c_str(), &texture_data.width, &texture_data.height, &texture_data.nrComponents, 0);

        if (texture_data.data)
        {
            if (texture_data.nrComponents == 1)
                texture_data.format = GL_RED;
            else if (texture_data.nrComponents == 3)
                texture_data.format = GL_RGB;
            else if (texture_data.nrComponents == 4)
                texture_data.format = GL_RGBA;
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(texture_data.data);
        }

        return texture_data;
    }

    auto OpenGL_Texture::create_texture() -> void
    {
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, texture_data.format, texture_data.width, texture_data.height, 0, texture_data.format, GL_UNSIGNED_BYTE, texture_data.data);

        //TODO: remove outside this func
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_data.format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_data.format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(texture_data.data);
    }


    auto OpenGL_Texture::get_texture_unit() -> unsigned int 
    {
        return texture_unit_;
    }
}