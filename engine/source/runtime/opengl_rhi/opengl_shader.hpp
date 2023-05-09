#pragma once
#include <glad/glad.h>
#include <string>
#include "rhi/rhi_defination.hpp"

namespace Helios
{
    enum struct Shader_Type
    {
        VERTEX_SHADER,
        FRAGMENT_SHADER
    };

    class OpenGL_Shader : public RHI_Shader
    {
    public:
        OpenGL_Shader(const std::string& in_vert_path, const std::string& in_frag_path);
        virtual ~OpenGL_Shader();
        auto bind() -> void override;
        auto set_uniform() -> void override;

    private:
        auto read_file(const std::string& path) -> std::string;
        auto create_gl_shader(const std::string& src, Shader_Type type) -> unsigned int;
        auto compile(unsigned int shader) -> void;
        auto link() -> void;
        unsigned int vertex_shader;
        unsigned int fragment_shader;
        Shader_Type type;
        GLuint resource;
    };
}