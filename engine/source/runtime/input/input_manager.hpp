#pragma once

#include <glm/glm.hpp>

namespace Helios
{
    enum class Control_Command : uint32_t
    {
        camera_left      = 1 << 0,  // A
        camera_back      = 1 << 1,  // S
        camera_foward    = 1 << 2,  // W
        camera_right     = 1 << 3,  // D
        camera_up        = 1 << 4,  // Q
        camera_down      = 1 << 5,  // E
        exit             = 1 << 6  // Esc
    };

    class Input_Manager
    {
    public:
        auto initialize() -> void;
        auto tick(float delta_time) -> void;

        auto process_control_command() -> void;

        auto onKey(int key, int scancode, int action, int mods) -> void;
        auto onReset() -> void;
        auto onCursorPos(double xpos, double ypos) -> void;
        auto onCursorEnter(int entered) -> void;
        auto onScroll(double xoffset, double yoffset) -> void;
        auto onMouseButtonClicked(int key, int action) -> void;
        auto onWindowClosed() -> void;

    private:
        glm::vec2 m_engine_window_size {1280.0f, 900.0f};
        float   m_mouse_x {0.0f};
        float   m_mouse_y {0.0f};
        float   m_camera_speed {0.05f};

        unsigned int m_process_command {0};
    };
}
