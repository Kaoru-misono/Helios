#pragma once
#include <glad/glad.h>
#include <string>
#include "rhi/rhi_defination.hpp"

namespace Helios
{
    class OpenGL_Shader : public RHI_Shader
    {
    public:
        OpenGL_Shader(const std::string& in_path);
        ~OpenGL_Shader() override;

        auto shader_id()const -> unsigned int override;
        auto shader_source()const -> std::string override;
        auto shader_type()const -> Shader_Type override;

        static auto create_gl_shader(const std::string& src, Shader_Type type) -> unsigned int;
        static auto compile(unsigned int shader, Shader_Type type) -> void;
        static auto read_file(const std::string& path, Shader_Type& type) -> std::string;

    private:
        std::string name;
        unsigned int shader_id_;
        std::string shader_source_;
        Shader_Type type_;
    };
}
