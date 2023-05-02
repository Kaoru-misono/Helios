#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include<spdlog/spdlog.h>

namespace Helios::core
{
    struct Window_Info
    {
        int width{ 1200 };
        int height{ 900 };
        const char* title{ "Helios" };
    };

    class Window
    {
    public:
        Window() = default;
        ~Window();

        auto init(Window_Info& info) -> void;
        auto should_close() -> bool;
        auto get_window() -> GLFWwindow*;

    private:
        GLFWwindow* m_window{ nullptr };
        int m_width{ 0 };
        int m_height{ 0 };
        const char* m_title{ "" };
    };    
}