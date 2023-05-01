#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace helios
{
	class Helios_Engine
	{
	public:
		Helios_Engine();
		virtual ~Helios_Engine();

		void start_engine();
		void shutdown_engine();

		void renderer_tick();
	private:
		std::shared_ptr<spdlog::logger> version;
		GLFWwindow* window;
	};
}