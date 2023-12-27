#include "input_manager.hpp"
#include "global_context/global_context.hpp"
#include "window/window.hpp"
#include "render/camera.hpp"
#include "logger/logger_marco.hpp"
#include <glm/gtc/quaternion.hpp>
#include <functional>

namespace Helios
{
    namespace
    {
        unsigned int reset_command = 0xFFFFFFFF;
    }

    auto Input_Manager::initialize() -> void
    {
        Window::instance().registerOnCursorPosFunc(
            std::bind(&Input_Manager::onCursorPos, this, std::placeholders::_1, std::placeholders::_2));
        Window::instance().registerOnCursorEnterFunc(
            std::bind(&Input_Manager::onCursorEnter, this, std::placeholders::_1));
        Window::instance().registerOnScrollFunc(
            std::bind(&Input_Manager::onScroll, this, std::placeholders::_1, std::placeholders::_2));
        Window::instance().registerOnMouseButtonFunc(
            std::bind(&Input_Manager::onMouseButtonClicked, this, std::placeholders::_1, std::placeholders::_2));
        Window::instance().registerOnWindowCloseFunc(
            std::bind(&Input_Manager::onWindowClosed, this));
        Window::instance().registerOnKeyFunc(std::bind(&Input_Manager::onKey,
                                                                             this,
                                                                             std::placeholders::_1,
                                                                             std::placeholders::_2,
                                                                             std::placeholders::_3,
                                                                             std::placeholders::_4));
    }

    auto Input_Manager::process_control_command() -> void
    {
        float           camera_speed  = m_camera_speed;
        std::shared_ptr<Scene::Camera> camera = g_global_context.m_main_camera;
        glm::vec3 camera_relative_pos(0, 0, 0);

        if ((unsigned int)Control_Command::camera_foward & m_process_command)
        {
            camera_relative_pos += camera->forward() * camera_speed;
        }
        if ((unsigned int)Control_Command::camera_back & m_process_command)
        {
            camera_relative_pos -= camera->forward() * camera_speed;
        }
        if ((unsigned int)Control_Command::camera_left & m_process_command)
        {
            camera_relative_pos -= camera->right() * camera_speed;
        }
        if ((unsigned int)Control_Command::camera_right & m_process_command)
        {
            camera_relative_pos += camera->right() * camera_speed;
        }
        if ((unsigned int)Control_Command::camera_up & m_process_command)
        {
            camera_relative_pos += camera->up() * camera_speed;
        }
        if ((unsigned int)Control_Command::camera_down & m_process_command)
        {
            camera_relative_pos -= camera->up() * camera_speed;
        }

        //TODO: remove this
        if ((unsigned int)Control_Command::exit & m_process_command)
        Window::instance().close_window();

        if(camera_relative_pos != glm::vec3(0.0f))
        camera->move(camera_relative_pos);
    }

    auto Input_Manager::onKey(int key, int scancode, int action, int mods) -> void
    {
        if (action == GLFW_PRESS)
        {
            switch (key)
            {
                case GLFW_KEY_ESCAPE:
                    m_process_command |= (unsigned int)Control_Command::exit;
                    break;
                case GLFW_KEY_A:
                    m_process_command |= (unsigned int)Control_Command::camera_left;
                    break;
                case GLFW_KEY_S:
                    m_process_command |= (unsigned int)Control_Command::camera_back;
                    break;
                case GLFW_KEY_W:
                    m_process_command |= (unsigned int)Control_Command::camera_foward;
                    break;
                case GLFW_KEY_D:
                    m_process_command |= (unsigned int)Control_Command::camera_right;
                    break;
                case GLFW_KEY_Q:
                    m_process_command |= (unsigned int)Control_Command::camera_up;
                    break;
                case GLFW_KEY_E:
                    m_process_command |= (unsigned int)Control_Command::camera_down;
                    break;
                default:
                    break;
            }
        }
        else if (action == GLFW_RELEASE)
        {
            switch (key)
            {
                case GLFW_KEY_ESCAPE:
                    m_process_command &= (reset_command ^ (unsigned int)Control_Command::exit);
                    break;
                case GLFW_KEY_A:
                    m_process_command &= (reset_command ^ (unsigned int)Control_Command::camera_left);
                    break;
                case GLFW_KEY_S:
                    m_process_command &= (reset_command ^ (unsigned int)Control_Command::camera_back);
                    break;
                case GLFW_KEY_W:
                    m_process_command &= (reset_command ^ (unsigned int)Control_Command::camera_foward);
                    break;
                case GLFW_KEY_D:
                    m_process_command &= (reset_command ^ (unsigned int)Control_Command::camera_right);
                    break;
                case GLFW_KEY_Q:
                    m_process_command &= (reset_command ^ (unsigned int)Control_Command::camera_up);
                    break;
                case GLFW_KEY_E:
                    m_process_command &= (reset_command ^ (unsigned int)Control_Command::camera_down);
                    break;
                default:
                    break;
            }
        }
    }

    auto Input_Manager::onReset() -> void
    {

    }

    auto Input_Manager::onCursorPos(double xpos, double ypos) -> void
    {
        //LOG_INFO("mouse position: {0}, {1}", xpos, ypos);
        float angularVelocity =180.0f / 1600.f; // 180 degrees while moving full screen
        if (Window::instance().isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
        {
            glfwSetInputMode(Window::instance().get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            //ypos decrease when mouse up
            g_global_context.m_main_camera->rotate(glm::vec2(xpos - m_mouse_x, ypos - m_mouse_y) * angularVelocity);
        }
        else
        {
            glfwSetInputMode(Window::instance().get_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        m_mouse_x = xpos;
        m_mouse_y = ypos;
    }

    auto Input_Manager::onCursorEnter(int entered) -> void
    {
        if (!entered) // lost focus
        {
            m_mouse_x = m_mouse_y = -1.0f;
        }
    }

    auto Input_Manager::onScroll(double xoffset, double yoffset) -> void
    {
        if (Window::instance().isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
            {
                if (yoffset > 0)
                {
                    m_camera_speed *= 1.2f;
                }
                else
                {
                    m_camera_speed *= 0.8f;
                }
            }
            else
            {
                g_global_context.m_main_camera->zoom(
                    (float)yoffset * 2.0f); // wheel scrolled up = zoom in by 2 extra degrees
            }
    }

    auto Input_Manager::onMouseButtonClicked(int key, int action) -> void
    {

    }

    auto Input_Manager::onWindowClosed() -> void
    {

    }
}
