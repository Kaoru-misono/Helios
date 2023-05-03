#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "global_context/global_context.hpp"
#include "logger/logger.hpp"
#include "rhi/rhi.hpp"

namespace Helios
{
	class Helios_Engine
	{
	public:
		Helios_Engine();
		virtual ~Helios_Engine();

		auto run() -> void;
		auto shutdown() -> void;

		auto renderer_tick() -> void;
	private:
	Global_Context& context = g_global_context;
	std::shared_ptr<RHI> m_rhi;
	};
}