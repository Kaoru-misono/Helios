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
        OpenGL_Texture();
        ~OpenGL_Texture() override;
        auto bind() const -> void override;
        auto unbind() const -> void override;
        auto set_sampler(Texture_Sampler const& sampler) const -> void override;
        auto id() -> GLuint { return resource; }

        auto copy_data_from_buffer(void* texture_data) const -> void override;

        static auto load_2D_texture(std::string const& path, bool flip = false) -> std::shared_ptr<OpenGL_Texture>;
        static auto load_cube_map_texture(std::vector<std::string> const& paths, bool flip = false) -> std::shared_ptr<OpenGL_Texture>;
        static auto create_texture(Texture::Kind kind, Texture::Format format, int width, int height) -> std::shared_ptr<OpenGL_Texture>;

        GLenum gl_kind;
        int width;
        int height;
    private:
        GLuint resource;
    };
}
