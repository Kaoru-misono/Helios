#pragma once
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        auto set_uniform(const std::string& name, const glm::mat4& values) -> void override;
        auto set_uniform(const std::string& name, const glm::vec3& values) -> void override;

        //TODO: save uniform to a map to avoid set a uniform two times

    private:
        auto attach_shader(const std::shared_ptr<RHI_Shader>& shader) -> void;
        auto detach_shader(const std::shared_ptr<RHI_Shader>& shader) -> void;
        auto link() ->void;
        std::shared_ptr<RHI_Shader> vertex_shader_;
        std::shared_ptr<RHI_Shader> fragment_shader_;
        
        GLuint resource;
    };
}