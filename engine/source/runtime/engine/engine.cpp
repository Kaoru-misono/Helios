#include "engine.hpp"
#include <iostream>
#include <memory>

helios::Helios_Engine::Helios_Engine():version(spdlog::stdout_color_mt("Helios")), window(nullptr)
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	version->info("Welcome To Helios !");
}

helios::Helios_Engine::~Helios_Engine()
{
}

void helios::Helios_Engine::start_engine()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "Helios", nullptr, nullptr);
	if (window == nullptr)
	{
		version->error("Failed to create window!");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		version->error("Failed to initialized Glad!");
		return;
	}
}

void helios::Helios_Engine::shutdown_engine()
{
	glfwTerminate();
}

void helios::Helios_Engine::renderer_tick()
{

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
