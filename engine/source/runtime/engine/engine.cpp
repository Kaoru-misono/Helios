#include "engine.hpp"
#include <iostream>
#include <memory>

#include "logger/logger.hpp"
#include "logger/logger_marco.hpp"
#include "window/window.hpp"

namespace Helios
{
	Helios_Engine::Helios_Engine()
	{	
	}

	Helios_Engine::~Helios_Engine()
	{
	}

	auto Helios_Engine::run() -> void
	{
		context.start_context();
		glfwMakeContextCurrent(context.m_window->get_window());

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_ERROR("Failed to initialized Glad!");
			return;
		}
		
		LOG_INFO("Welcome to Helios !");
	}

	auto Helios_Engine::shutdown() -> void
	{
		glfwTerminate();
		context.shutdown_context();
	}

	auto Helios_Engine::renderer_tick() -> void
	{
		auto& window = context.m_window;
		while (!window->should_close())
		{
			glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glfwSwapBuffers(window->get_window());
			glfwPollEvents();
		}
	}
}
