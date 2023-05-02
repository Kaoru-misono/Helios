#include "global_context.hpp"
#include "core/logger/logger.hpp"
namespace Helios::core
{
    Global_Context g_global_context;

    auto Global_Context::start_context() -> void
    {
        m_logger = std::make_shared<Logger>();
    }

    auto Global_Context::shutdown_context() -> void
    {
        m_logger.reset();
    }
}