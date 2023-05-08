#pragma once
#include <glad/glad.h>
#include <string>
#include "rhi/rhi_defination.hpp"

namespace Helios
{
    class OpenGL_Shader : public RHI_Shader
    {
    public:
        OpenGL_Shader(const std::string& in_vert_path, const std::string& in_frag_path);
        virtual ~OpenGL_Shader();
        auto bind() -> void override;
        auto set_uniform() -> void override;

    private:
        auto read_file(const std::string& path) -> std::string;
        auto compile(const std::string& vert_path, const std::string& frag_path) -> void;
        GLuint resource;
    };
}