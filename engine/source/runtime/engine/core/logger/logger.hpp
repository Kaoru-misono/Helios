#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <memory>
#include <cstdint>

namespace Helios::core
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

        template<typename... TARGS>
        auto log(Log_Level level, TARGS&&... args) -> void
        {
            switch (level)
            {
                case Log_Level::trace:
                    m_logger->trace(std::forward<TARGS>(args)...);
                    break;
                case Log_Level::debug:
                    m_logger->debug(std::forward<TARGS>(args)...);
                    break;
                case Log_Level::info:
                    m_logger->info(std::forward<TARGS>(args)...);
                    break;
                case Log_Level::warn:
                    m_logger->warn(std::forward<TARGS>(args)...);
                    break;
                case Log_Level::error:
                    m_logger->error(std::forward<TARGS>(args)...);
                    break;
                case Log_Level::fetal:
                    m_logger->critical(std::forward<TARGS>(args)...);
                    fatalCallback(std::forward<TARGS>(args)...);
                    break;
                default:
                    break;
            }
        }

        template<typename... TARGS>
        void fatalCallback(TARGS&&... args)
        {
            const std::string format_str = fmt::format(std::forward<TARGS>(args)...);
            throw std::runtime_error(format_str);
        }

    private:
        std::shared_ptr<spdlog::logger> m_logger;
    };
}