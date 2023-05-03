#include "engine.hpp"
#include <iostream>
#include <memory>

#include "logger/logger.hpp"
#include "logger/logger_marco.hpp"
#include "window/window.hpp"
#include "opengl_rhi/opengl_rhi.hpp"

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
		m_rhi = std::make_shared<OpenGL_RHI>();
		m_rhi->initialize(context.m_window);
		m_rhi->create_context();
		LOG_INFO("Welcome to Helios !");
		renderer_tick();
	}

	auto Helios_Engine::shutdown() -> void
	{
		
		context.shutdown_context();
	}

	auto Helios_Engine::renderer_tick() -> void
	{
		auto& window = context.m_window;
		while (!window->should_close())
		{
			glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			window->swap_buffers();
			window->poll_events();
		}
	}
}
