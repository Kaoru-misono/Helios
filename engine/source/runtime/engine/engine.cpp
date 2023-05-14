#include "engine.hpp"
#include <iostream>
#include <memory>
#include <glm/glm.hpp>

#include "logger/logger.hpp"
#include "logger/logger_marco.hpp"
#include "window/window.hpp"
#include "opengl_rhi/opengl_rhi.hpp"
#include "rhi/rhi_defination.hpp"
#include "opengl_rhi/opengl_gpu_program.hpp"
#include "render/camera.hpp"


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
			-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
			 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 
			-0.5,  0.5, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0,
			 0.5,  0.5, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0
		};

		glm::vec3 position{1.0f, 1.0f, 1.0f};
		
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 1
		};
		
		RHI_Buffer_Create_info info;
		info.data_array = std::make_shared<Data_Array>(sizeof(vertices), vertices);
		std::shared_ptr<RHI_Buffer> vertex_buffer = m_rhi->create_buffer(info, RHI_Usage_Flag::vertex_buffer, info.data_array->size, 0);
		
		Vertex_Array_Specifier specifier{
			{ "POSITION", Vertex_Attribute_Type::Float3 },
			{ "COLOR", 	  Vertex_Attribute_Type::Float3 },
			{ "TEXCOORD", Vertex_Attribute_Type::Float2 }
		};
		vertex_array->set_attributes(specifier);

		RHI_Buffer_Create_info index_info;
		index_info.data_array = std::make_shared<Data_Array>(sizeof(indices), indices);
		std::shared_ptr<RHI_Buffer> index_buffer = m_rhi->create_buffer(index_info, RHI_Usage_Flag::index_buffer, index_info.data_array->size, 0);

		std::shared_ptr<RHI_Shader> vertex_shader = m_rhi->create_shader( "shader/test_vert.glsl");
		std::shared_ptr<RHI_Shader> fragment_shader = m_rhi->create_shader( "shader/test_frag.glsl");
		
		std::shared_ptr<RHI_Texture> leidian = m_rhi->create_texture( "texture/leidian.jpg" );

		leidian->set_texture_unit(0);
		std::shared_ptr<RHI_GPU_Program> pass = std::make_shared<OpenGL_GPU_Program>();
		pass->add_vertex_shader(vertex_shader);
		pass->add_fragment_shader(fragment_shader);
		pass->link_shader();
		
		Scene::Camera camera;
		camera.set_camera_properties(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f));

		pass->set_uniform("view_matrix", camera.get_view_matrix());
		pass->set_uniform("projection_matrix", camera.get_projection_matrix());


		
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
