#pragma once
#include <glad/glad.h>
#include "rhi/rhi_defination.hpp"



namespace Helios
{
    using namespace rhi;

    class OpenGL_Texture : public RHI_Texture
    {
    public:
        struct Texture_Data
        {
            unsigned char* data;
            GLenum format;
            int width, height, nrComponents;
        };

        OpenGL_Texture(const std::string& path);
        ~OpenGL_Texture() override;

        auto set_texture_sampler(unsigned int texture_unit) -> void override;
        auto get_texture_unit() -> unsigned int;
        
        static auto load_texture(const std::string& path) -> Texture_Data;
        
        auto create_texture() -> void;

    private:
        GLuint resource;
        Texture_Data texture_data;
        unsigned int texture_unit_;
    };
}