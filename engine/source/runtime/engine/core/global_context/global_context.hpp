#pragma once
#include <memory>
#include <string>
namespace Helios::core
{
    class Logger;

    class Global_Context
    {
    public:
        auto start_context() -> void;
        auto shutdown_context() -> void;

    public:
        std::shared_ptr<Logger> m_logger;
    };

    extern Global_Context g_global_context;
}