#include "engine.hpp"
#include <iostream>
#include <memory>

#include "logger/logger.hpp"
#include "logger/logger_marco.hpp"
#include "window/window.hpp"
#include "opengl_rhi/opengl_rhi.hpp"
#include "rhi/rhi_defination.hpp"

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
		m_rhi->init(context.m_window);
		m_rhi->create_context();
		LOG_INFO("Welcome to Helios !");

		float vertices[] = {
			1.0, 1.0, 1.0
		};
		RHI_Buffer_Create_info info;
		info.data_array = std::make_shared<Data_Array>();
		info.data_array->data = &vertices[0];
		info.data_array->size = sizeof(vertices);
		std::shared_ptr<RHI_Buffer> buffer = m_rhi->create_buffer(info, RHI_Usage_Flag::vertex_buffer, info.data_array->size, 0);
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
