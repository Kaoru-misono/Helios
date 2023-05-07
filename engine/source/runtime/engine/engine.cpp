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
		std::shared_ptr<RHI_Vertex_Array> vertex_array = m_rhi->create_vertex_array();

		float vertices[] = {
			-1.0, -1.0, 
			 1.0, -1.0,
			 0.0,  1.0
		};
		RHI_Buffer_Create_info info;
		info.data_array = std::make_shared<Data_Array>(sizeof(vertices), vertices);
		std::shared_ptr<RHI_Buffer> buffer = m_rhi->create_buffer(info, RHI_Usage_Flag::vertex_buffer, info.data_array->size, 0);
		Vertex_Array_specifier specifier;
		specifier.attributes_.clear();
		Vertex_Attribute_Descriptor attribute;
		{
			attribute.element_name = "POSITION";
			attribute.offset = 0;
			attribute.type = Vertex_Attribute_Type::Float2;
			attribute.buffer_token = buffer;
		}
		specifier.attributes_.emplace_back(attribute);
		specifier.stride = 2;
		vertex_array->set_attributes(specifier);
		
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
			glDrawArrays(GL_TRIANGLES, 0, 3);

			window->swap_buffers();
			window->poll_events();
		}
	}
}
