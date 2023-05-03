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
        auto initialize(std::shared_ptr<Window>& window) -> void override final;
        auto create_context() -> void override final;
    private:
        GLFWwindow* _window;
    };
}