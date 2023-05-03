#pragma once
#include "logger.hpp"
#include "global_context/global_context.hpp"
#include <memory>
#include <chrono>
#include <thread>

using Log_Level = Helios::Logger::Log_Level;
#define LOG_HELPER(LOG_LEVEL, ...) \
g_global_context.m_logger ->log(LOG_LEVEL, __VA_ARGS__);

#define LOG_DEBUG(...) LOG_HELPER(Log_Level::debug, __VA_ARGS__);

#define LOG_INFO(...) LOG_HELPER(Log_Level::info, __VA_ARGS__);

#define LOG_WARN(...) LOG_HELPER(Log_Level::warn, __VA_ARGS__);

#define LOG_ERROR(...) LOG_HELPER(Log_Level::error, __VA_ARGS__);

#define LOG_FATAL(...) LOG_HELPER(Log_Level::fatal, __VA_ARGS__);

#define PolitSleep(_ms) std::this_thread::sleep_for(std::chrono::milliseconds(_ms));