#pragma once
#include <glad/glad.h>
#include "rhi/rhi_defination.hpp"
#include "resource/image.hpp"


namespace Helios
{
    class OpenGL_Texture : public RHI_Texture
    {
    public:
        std::vector<std::shared_ptr<Image>> images;

        OpenGL_Texture();
        ~OpenGL_Texture() override;

        auto texture_id() const -> unsigned int override { return resource; }

        static auto load_2D_texture(std::string path, bool flip = false) -> std::shared_ptr<OpenGL_Texture>;
    private:
        GLuint resource;
    };
}
