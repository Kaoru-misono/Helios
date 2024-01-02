#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <memory>
#include <cstdint>

namespace Helios
{
    class Logger final
    {
    public:
        enum struct Log_Level : uint8_t
        {
            trace,
            debug,
            info,
            warn,
            error,
            fetal
        };
        public:
        Logger();
        ~Logger();

        template<typename... Args>
        auto log(Log_Level level, fmt::format_string<Args...> fmt, Args&&... args) -> void
        {
            switch (level)
            {
                case Log_Level::trace:
                    m_logger->trace(fmt, std::forward<Args>(args)...);
                    break;
                case Log_Level::debug:
                    m_logger->debug(fmt, std::forward<Args>(args)...);
                    break;
                case Log_Level::info:
                    m_logger->info(fmt, std::forward<Args>(args)...);
                    break;
                case Log_Level::warn:
                    m_logger->warn(fmt, std::forward<Args>(args)...);
                    break;
                case Log_Level::error:
                    m_logger->error(fmt, std::forward<Args>(args)...);
                    break;
                case Log_Level::fetal:
                    m_logger->critical(fmt, std::forward<Args>(args)...);
                    fatalCallback(std::forward<Args>(args)...);
                    break;
                default:
                    break;
            }
        }

        template<typename... Args>
        void fatalCallback(Args&&... args)
        {
            //const std::string format_str = fmt::format(std::forward<Args>(args)...);
            //throw std::runtime_error(format_str);
        }

    private:
        std::shared_ptr<spdlog::logger> m_logger;
    };
}