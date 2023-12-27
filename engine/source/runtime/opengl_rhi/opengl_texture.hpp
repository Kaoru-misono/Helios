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
        auto bind() const -> void override;
        auto unbind() const -> void override;
        auto set_sampler(Texture_Sampler const& sampler, int texture_unit) const -> void override;

        static auto load_2D_texture(std::string const& path, bool flip = false) -> std::shared_ptr<OpenGL_Texture>;
        static auto load_cube_map_texture(std::vector<std::string> const& paths, bool flip = false) -> std::shared_ptr<OpenGL_Texture>;
        GLenum gl_kind;
    private:
        GLuint resource;
    };
}
