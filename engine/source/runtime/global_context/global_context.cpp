#include "global_context.hpp"
#include "logger/logger.hpp"
#include "window/window.hpp"
namespace Helios
{
    Global_Context g_global_context;

    auto Global_Context::start_context() -> void
    {
        m_logger = std::make_shared<Logger>();

        m_window = std::make_shared<Window>();
        Window_Info info;
        m_window->init(info);
    }

    auto Global_Context::shutdown_context() -> void
    {
        m_logger.reset();
    }
}