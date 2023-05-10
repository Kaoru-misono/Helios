#pragma once
#include <glad/glad.h>
#include "rhi/rhi_defination.hpp"

namespace Helios
{
    class OpenGL_GPU_Program : public RHI_GPU_Program
    {
    public:
        OpenGL_GPU_Program();
        virtual ~OpenGL_GPU_Program() override;
        auto bind() -> void override;
        auto add_vertex_shader(const std::shared_ptr<RHI_Shader>& vertex_shader) -> void override;
        auto add_fragment_shader(const std::shared_ptr<RHI_Shader>& fragment_shader) -> void override;
        auto link_shader() -> void override;

    private:
        auto attach_shader(const std::shared_ptr<RHI_Shader>& shader) -> void;
        auto detach_shader(const std::shared_ptr<RHI_Shader>& shader) -> void;
        auto link() ->void;
        std::shared_ptr<RHI_Shader> vertex_shader_;
        std::shared_ptr<RHI_Shader> fragment_shader_;
        GLuint resource;
    };
}