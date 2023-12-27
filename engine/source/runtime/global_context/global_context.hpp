#pragma once
#include <memory>
#include <string>
#include "render/camera.hpp"
namespace Helios
{
    using namespace Scene;

    class Logger;
    class Window;
    class ImGui_Layer;

    class Global_Context
    {
    public:
        auto start_context() -> void;
        auto shutdown_context() -> void;

    public:
        std::shared_ptr<Logger> m_logger;
        std::shared_ptr<ImGui_Layer> m_imgui_layer;
        std::shared_ptr<Camera> m_main_camera;
    };

    extern Global_Context g_global_context;
}
