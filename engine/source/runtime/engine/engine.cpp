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
		//GLuint vertex_array;
		//glGenVertexArrays(1, &vertex_array);

		float vertices[] = {
			-0.5, -0.5, 1.0, 0.0, 0.0,
			 0.5, -0.5, 0.0, 1.0, 0.0, 
			-0.5,  0.5, 0.0, 0.0, 1.0,
			 0.5,  0.5, 1.0, 1.0, 1.0
		};
		
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 1
		};
		
		RHI_Buffer_Create_info info;
		info.data_array = std::make_shared<Data_Array>(sizeof(vertices), vertices);
		std::shared_ptr<RHI_Buffer> vertex_buffer = m_rhi->create_buffer(info, RHI_Usage_Flag::vertex_buffer, info.data_array->size, 0);
		
		Vertex_Array_Specifier specifier{
			{ "POSITION", Vertex_Attribute_Type::Float2 },
			{ "COLOR", Vertex_Attribute_Type::Float3 }
		};
		vertex_array->set_attributes(specifier);

		RHI_Buffer_Create_info index_info;
		index_info.data_array = std::make_shared<Data_Array>(sizeof(indices), indices);
		std::shared_ptr<RHI_Buffer> index_buffer = m_rhi->create_buffer(index_info, RHI_Usage_Flag::index_buffer, index_info.data_array->size, 0);

		std::shared_ptr<RHI_Shader> shader = m_rhi->create_shader( "C:/Users/30931/Desktop/Helios/Helios/engine/asset/shader/test_vert.glsl",
																   "C:/Users/30931/Desktop/Helios/Helios/engine/asset/shader/test_frag.glsl");
		shader->bind();
		while (!context.m_window->should_close())
		{
			renderer_tick();
		}
	}

	auto Helios_Engine::shutdown() -> void
	{
		
		context.shutdown_context();
	}

	auto Helios_Engine::renderer_tick() -> void
	{
		
		glClearColor(0.8f, 0.5f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		context.m_window->swap_buffers();
		context.m_window->poll_events();
	}
}
