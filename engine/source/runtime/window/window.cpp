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
	    
        
        m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);

        if (m_window == nullptr)
	    {
	    	LOG_ERROR("Failed to create window !");
	    	glfwTerminate();
	    	return;
	    }
    }
    
    auto Window::should_close() -> bool
    {
        return glfwWindowShouldClose(m_window);
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