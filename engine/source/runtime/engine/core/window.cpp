#include "window.hpp"
#include <iostream>

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
            std::cout << "Failed to initialized GLFW !" << std::endl;
            return;
        }

        m_width = info.width;
        m_height = info.height;
        m_title = info.title;
	    
        m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);

        if (m_window == nullptr)
	    {
	    	std::cout << "Failed to create window !" << std::endl;
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
}