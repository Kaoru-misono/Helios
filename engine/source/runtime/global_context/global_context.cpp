#include "global_context.hpp"
#include "logger/logger.hpp"
#include "window/window.hpp"
#include "imgui_layer/imgui_layer.hpp"
#include "render/camera.hpp"

namespace Helios
{
    Global_Context g_global_context;

    auto Global_Context::start_context() -> void
    {
        m_logger = std::make_shared<Logger>();

        m_window = std::make_shared<Window>();
        Window_Info info;
        m_window->init(info);

        m_imgui_layer = std::make_shared<ImGui_Layer>();
        m_imgui_layer->init();

        m_main_camera = std::make_shared<Camera>();
    }

    auto Global_Context::shutdown_context() -> void
    {
        m_logger.reset();

        m_window.reset();

        m_imgui_layer->destroy();
        m_imgui_layer.reset();
    }
}
