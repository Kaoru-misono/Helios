#pragma once
#include "rhi/rhi.hpp"
#include "window/window.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "opengl_shader.hpp"

namespace Helios
{
    class OpenGL_RHI final : public RHI
    {
    public:
        virtual ~OpenGL_RHI() override final;
        auto init(Window& window) -> void override final;
        auto create_platform_context() -> void override final;
        auto init_imgui_for_platform() -> void override final;

        auto create_buffer(RHI_Buffer_Create_info& buffer_create_info, RHI_Usage_Flag flag) -> std::shared_ptr<RHI_Buffer> override;
        auto create_vertex_array() -> std::shared_ptr<RHI_Vertex_Array> override;
        auto create_shader(const std::string& path) -> std::shared_ptr<RHI_Shader> override;
        auto create_texture(Texture::Kind kind, std::vector<std::string> const& paths, Texture::Format format = Texture::Format::rgb8, bool should_flip = false) -> std::shared_ptr<Texture> override;
        auto create_framebuffer() -> std::shared_ptr<RHI_Framebuffer> override;

    private:
        GLFWwindow* window_;
    };

}
