#include "window.hpp"
#include <iostream>
#include "logger/logger_marco.hpp"
#include "global_context/global_context.hpp"

namespace Helios
{
    Window::~Window()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    auto Window::init(Window_Info& info) -> void
    {
        
        if(!glfwInit())
        {
            LOG_ERROR("Failed to initialized GLFW !");
            return;
        }

        m_width = info.width;
        m_height = info.height;
        m_title = info.title;
	    
        
        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

        if (m_window == nullptr)
	    {
	    	LOG_ERROR("Failed to create window !");
	    	glfwTerminate();
	    	return;
	    }

        // Setup input callbacks
        glfwSetWindowUserPointer(m_window, this); //window access the GLFWwindow m_Window
        glfwSetKeyCallback(m_window, key_callback);
        glfwSetCharCallback(m_window, char_callback);
        glfwSetCharModsCallback(m_window, char_mods_callback);
        glfwSetMouseButtonCallback(m_window, mouse_button_callback);
        glfwSetCursorPosCallback(m_window, cursor_pos_callback);
        glfwSetCursorEnterCallback(m_window, cursor_enter_callback);
        glfwSetScrollCallback(m_window, scroll_callback);
        glfwSetWindowSizeCallback(m_window, window_size_callback);
        glfwSetWindowCloseCallback(m_window, window_close_callback);
    }
    
    auto Window::should_close() -> bool
    {
        return glfwWindowShouldClose(m_window);
    }

    auto Window::close_window() -> void
    {
        glfwSetWindowShouldClose(m_window, true);
    }
    
    auto Window::get_window() -> GLFWwindow*
    {
        return m_window;
    }

    auto Window::swap_buffers() -> void
    {
        glfwSwapBuffers(m_window);
    }

    auto Window::poll_events() -> void
    {
        glfwPollEvents();
    }
}