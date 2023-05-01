#include "engine.hpp"
#include <iostream>
#include <memory>

namespace Helios
{
	
	Helios_Engine::Helios_Engine():version(spdlog::stdout_color_mt("Helios"))
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		version->info("Welcome To Helios !");
	}

	Helios_Engine::~Helios_Engine()
	{
	}

	auto Helios_Engine::start_engine() -> void
	{
		Window_Info info;
		_window.init(info);
		glfwMakeContextCurrent(_window.get_window());

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			version->error("Failed to initialized Glad!");
			return;
		}
	}

	auto Helios_Engine::shutdown_engine() -> void
	{
		glfwTerminate();
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
