#pragma once
#include "rhi/rhi.hpp"
#include "window/window.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Helios
{
    class OpenGL_RHI final : public RHI
    {
    public:
        virtual ~OpenGL_RHI() override final;
        auto init(std::shared_ptr<Window>& window) -> void override final;
        auto create_context() -> void override final;

        auto create_buffer(RHI_Buffer_Create_info& buffer_create_info, RHI_Usage_Flag flag, uint32_t size = 0, uint32_t stride = 0) -> std::shared_ptr<RHI_Buffer> override;
        auto create_vertex_array() -> std::shared_ptr<RHI_Vertex_Array> override;
    private:
        GLFWwindow* window_;
    };
}