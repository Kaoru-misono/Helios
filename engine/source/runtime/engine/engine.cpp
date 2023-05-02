#include "engine.hpp"
#include <iostream>
#include <memory>
#include "core/logger/logger.hpp"
#include "core/logger/logger_marco.hpp"
#include "core/global_context/global_context.hpp"

namespace Helios
{
	Helios_Engine::Helios_Engine()/*version(spdlog::stdout_color_mt("Helios"))*/
	{
		//spdlog::set_pattern("%^[%T] %n: %v%$");
		//version->info("Welcome To Helios !");
		
	}

	Helios_Engine::~Helios_Engine()
	{
	}

	auto Helios_Engine::start_engine() -> void
	{
		core::g_global_context.start_context();
		core::Window_Info info;
		_window.init(info);
		glfwMakeContextCurrent(_window.get_window());

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_ERROR("Failed to initialized Glad!");
			return;
		}
		
		LOG_INFO("Welcome to Helios !");
	}

	auto Helios_Engine::shutdown_engine() -> void
	{
		glfwTerminate();
		core::g_global_context.shutdown_context();
	}

	auto Helios_Engine::renderer_tick() -> void
	{

		while (!_window.should_close())
		{
			glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glfwSwapBuffers(_window.get_window());
			glfwPollEvents();
		}
	}
}
