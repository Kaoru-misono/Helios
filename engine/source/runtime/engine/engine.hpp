#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/window.hpp"
#include "core/logger/logger.hpp"

namespace Helios
{
	class Helios_Engine
	{
	public:
		Helios_Engine();
		virtual ~Helios_Engine();

		auto start_engine() -> void;
		auto shutdown_engine() -> void;

		auto renderer_tick() -> void;
	private:
		core::Window _window{};
	};
}