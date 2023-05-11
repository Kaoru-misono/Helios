#pragma once
#include <glad/glad.h>
#include "rhi/rhi_defination.hpp"



namespace Helios
{
    class OpenGL_Texture : public RHI_Texture
    {
    public:
        OpenGL_Texture(const std::string& path);
        ~OpenGL_Texture() override;

        auto set_texture_unit(unsigned int texture_unit) -> void override;

        static auto load_texture(const std::string& path, GLenum format) -> GLuint;

        auto get_texture_unit() -> unsigned int;

    private:
        GLuint resource;
        GLenum format;
        unsigned int texture_unit_;
    };
}