#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include<spdlog/spdlog.h>

namespace Helios
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
        auto close_window() -> void;
        auto get_window() -> GLFWwindow*;
        auto get_width() -> int { return m_width; }
        auto get_height() -> int { return m_height; }
        auto swap_buffers() -> void;
        auto poll_events() -> void;

        using reset_func        = std::function<void()>;
        using key_func          = std::function<void(int, int, int, int)>;
        using char_func         = std::function<void(unsigned int)>;
        using char_mods_func    = std::function<void(int, unsigned int)>;
        using mouse_button_func = std::function<void(int, int, int)>;
        using cursor_pos_func   = std::function<void(double, double)>;
        using cursor_enter_func = std::function<void(int)>;
        using scroll_func       = std::function<void(double, double)>;
        using window_size_func  = std::function<void(int, int)>;
        using window_close_func = std::function<void()>;

        void registerOnResetFunc(reset_func func) { m_reset_func.push_back(func); }
        void registerOnKeyFunc(key_func func) { m_key_func.push_back(func); }
        void registerOnCharFunc(char_func func) { m_char_func.push_back(func); }
        void registerOnCharModsFunc(char_mods_func func) { m_char_mods_func.push_back(func); }
        void registerOnMouseButtonFunc(mouse_button_func func) { m_mouse_button_func.push_back(func); }
        void registerOnCursorPosFunc(cursor_pos_func func) { m_cursor_pos_func.push_back(func); }
        void registerOnCursorEnterFunc(cursor_enter_func func) { m_cursor_enter_func.push_back(func); }
        void registerOnScrollFunc(scroll_func func) { m_scroll_func.push_back(func); }
        void registerOnWindowSizeFunc(window_size_func func) { m_window_size_func.push_back(func); }
        void registerOnWindowCloseFunc(window_close_func func) { m_window_close_func.push_back(func); }

        bool isMouseButtonDown(int button) const
        {
            if (button < GLFW_MOUSE_BUTTON_1 || button > GLFW_MOUSE_BUTTON_LAST)
            {
                return false;
            }
            return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
        }


    protected:
         // window event callbacks
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            Window* app = (Window*)glfwGetWindowUserPointer(window);
            if (app)
            {
                app->onKey(key, scancode, action, mods);
            }
        }
        static void char_callback(GLFWwindow* window, unsigned int codepoint)
        {
            Window* app = (Window*)glfwGetWindowUserPointer(window);
            if (app)
            {
                app->onChar(codepoint);
            }
        }
        static void char_mods_callback(GLFWwindow* window, unsigned int codepoint, int mods)
        {
            Window* app = (Window*)glfwGetWindowUserPointer(window);
            if (app)
            {
                app->onCharMods(codepoint, mods);
            }
        }
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
        {
            Window* app = (Window*)glfwGetWindowUserPointer(window);
            if (app)
            {
                app->onMouseButton(button, action, mods);
            }
        }
        static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
        {
            Window* app = (Window*)glfwGetWindowUserPointer(window);
            if (app)
            {
                app->onCursorPos(xpos, ypos);
            }
        }
        static void cursor_enter_callback(GLFWwindow* window, int entered)
        {
            Window* app = (Window*)glfwGetWindowUserPointer(window);
            if (app)
            {
                app->onCursorEnter(entered);
            }
        }
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
        {
            Window* app = (Window*)glfwGetWindowUserPointer(window);
            if (app)
            {
                app->onScroll(xoffset, yoffset);
            }
        }

        static void window_size_callback(GLFWwindow* window, int width, int height)
        {
            Window* app = (Window*)glfwGetWindowUserPointer(window);
            if (app)
            {
                app->m_width  = width;
                app->m_height = height;
            }
        }

        static auto framebuffer_size_callback(GLFWwindow* window, int width, int height)
        {
             glViewport(0, 0, width, height);
        }

        static void window_close_callback(GLFWwindow* window) { glfwSetWindowShouldClose(window, true); }

        void onReset()
        {
            for (auto& func : m_reset_func)
                func();
        }

        void onKey(int key, int scancode, int action, int mods)
        {
            for (auto& func : m_key_func)
                func(key, scancode, action, mods);
        }

        void onChar(unsigned int codepoint)
        {
            for (auto& func : m_char_func)
                func(codepoint);
        }

        void onCharMods(int codepoint, unsigned int mods)
        {
            for (auto& func : m_char_mods_func)
                func(codepoint, mods);
        }

        void onMouseButton(int button, int action, int mods)
        {
            for (auto& func : m_mouse_button_func)
                func(button, action, mods);
        }

        void onCursorPos(double xpos, double ypos)
        {
            for (auto& func : m_cursor_pos_func)
                func(xpos, ypos);
        }

        void onCursorEnter(int entered)
        {
            for (auto& func : m_cursor_enter_func)
                func(entered);
        }

        void onScroll(double xoffset, double yoffset)
        {
            for (auto& func : m_scroll_func)
                func(xoffset, yoffset);
        }

        void onWindowSize(int width, int height)
        {
            for (auto& func : m_window_size_func)
                func(width, height);
        }

    private:
        GLFWwindow* m_window{ nullptr };
        int m_width{ 0 };
        int m_height{ 0 };
        std::string m_title{ "" };

        std::vector<reset_func>        m_reset_func;
        std::vector<key_func>          m_key_func;
        std::vector<char_func>         m_char_func;
        std::vector<char_mods_func>    m_char_mods_func;
        std::vector<mouse_button_func> m_mouse_button_func;
        std::vector<cursor_pos_func>   m_cursor_pos_func;
        std::vector<cursor_enter_func> m_cursor_enter_func;
        std::vector<scroll_func>       m_scroll_func;
        std::vector<window_size_func>  m_window_size_func;
        std::vector<window_close_func> m_window_close_func;
    };
}
