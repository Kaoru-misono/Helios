#pragma once
#include <glad/glad.h>
#include <string>
#include "rhi/rhi_defination.hpp"

namespace Helios
{
    using namespace rhi;
    
    class OpenGL_Shader : public RHI_Shader
    {
    public:
        OpenGL_Shader(const std::string& in_path);
        virtual ~OpenGL_Shader() override;

        auto get_shader_id()const -> unsigned int override;
        auto get_shader_source()const -> std::string override;
        auto get_shader_type()const -> Shader_Type override;

        static auto create_gl_shader(const std::string& src, Shader_Type type) -> unsigned int;
        static auto compile(unsigned int shader, Shader_Type type) -> void;
        static auto read_file(const std::string& path, Shader_Type& type) -> std::string;

    private:
        unsigned int shader_id;
        std::string shader_source;
        Shader_Type type_;
    };
}