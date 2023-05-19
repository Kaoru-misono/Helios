#pragma once
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_set>

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
        auto set_uniform(const std::string& name, std::any const& value) -> void override;

    private:
        auto attach_shader(const std::shared_ptr<RHI_Shader>& shader) -> void;
        auto detach_shader(const std::shared_ptr<RHI_Shader>& shader) -> void;
        auto link() ->void;

         template<typename T>
        auto set_uniform_impl(std::string const& name, T const& value) -> void;

        template<>
        auto set_uniform_impl(std::string const& name, int const& value) -> void
        {
            GLint location = glGetUniformLocation(resource, name.c_str());
	        glUniform1i(location, value);
        }

        template<>
        auto set_uniform_impl(std::string const& name, float const& value) -> void
        {
            GLint location = glGetUniformLocation(resource, name.c_str());
	        glUniform1f(location, value);
        }

        template<>
        auto set_uniform_impl(std::string const& name, glm::vec2 const& value) -> void
        {
            GLint location = glGetUniformLocation(resource, name.c_str());
	        glUniform2f(location, value.x, value.y);
        }

        template<>
        auto set_uniform_impl(std::string const& name, glm::vec3 const& value) -> void
        {
            GLint location = glGetUniformLocation(resource, name.c_str());
	        glUniform3f(location, value.x, value.y, value.z);
        }

        template<>
        auto set_uniform_impl(std::string const& name, glm::vec4 const& value) -> void
        {
            GLint location = glGetUniformLocation(resource, name.c_str());
	        glUniform4f(location, value.x, value.y, value.z, value.w);
        }

        template<>
        auto set_uniform_impl(std::string const& name, glm::mat3 const& value) -> void
        {
            GLint location = glGetUniformLocation(resource, name.c_str());
	        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }

        template<>
        auto set_uniform_impl(std::string const& name, glm::mat4 const& value) -> void
        {
            GLint location = glGetUniformLocation(resource, name.c_str());
	        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }
        std::shared_ptr<RHI_Shader> vertex_shader_;
        std::shared_ptr<RHI_Shader> fragment_shader_;
        std::unordered_set<std::string> uniforms;
        
        GLuint resource;
    };
}