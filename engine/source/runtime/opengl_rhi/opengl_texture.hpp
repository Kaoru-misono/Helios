#pragma once
#include <glad/glad.h>
#include "rhi/rhi_defination.hpp"
#include "resource/image.hpp"
#include "rhi/texture.hpp"

namespace Helios
{
    class OpenGL_Texture : public Texture
    {
    public:
        std::vector<std::shared_ptr<Image>> images;

        OpenGL_Texture();
        ~OpenGL_Texture() override;

        auto texture_id() const -> unsigned int override { return resource; }

        static auto load_2D_texture(std::string const& path, bool flip = false) -> std::shared_ptr<OpenGL_Texture>;
    private:
        GLuint resource;
    };
}
