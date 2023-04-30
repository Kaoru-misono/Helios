#include "engine.hpp"
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

helios::Helios_Engine::Helios_Engine()
{
}

helios::Helios_Engine::~Helios_Engine()
{
}

void helios::Helios_Engine::start_engine()
{
}

void helios::Helios_Engine::shutdown_engine()
{
}

void helios::Helios_Engine::renderer_tick()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	std::shared_ptr<spdlog::logger> version = spdlog::stdout_color_mt("Helios");
	version->set_level(spdlog::level::trace);
	version->trace("Hello Helios!");
	version->warn("Hello Helios!");
	version->error("Hello Helios!");
	version->info("Hello Helios!");
	version->critical("Hello Helios!");

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Helios", nullptr, nullptr);
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

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}
