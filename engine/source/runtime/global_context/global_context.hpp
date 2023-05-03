#pragma once
#include <memory>
#include <string>
namespace Helios
{
    class Logger;
    class Window;

    class Global_Context
    {
    public:
        auto start_context() -> void;
        auto shutdown_context() -> void;

    public:
        std::shared_ptr<Logger> m_logger;
        std::shared_ptr<Window> m_window;
    };

    extern Global_Context g_global_context;
}