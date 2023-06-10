#include "engine.hpp"
#include <iostream>
#include <memory>
#include <any>
#include <glm/glm.hpp>
#include <filesystem>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/gtc/type_ptr.hpp>

#include "logger/logger.hpp"
#include "logger/logger_marco.hpp"
#include "window/window.hpp"
#include "opengl_rhi/opengl_rhi.hpp"
#include "rhi/rhi_defination.hpp"
#include "opengl_rhi/opengl_gpu_program.hpp"
#include "render/camera.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <q_model.hpp>
#include "import_assimp_model/import_assimp_model.hpp"


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
		m_rhi->create_platform_context();
		LOG_INFO("Welcome to Helios !");
		m_rhi->init_imgui_for_platform();

		m_input_manager = std::make_shared<Input_Manager>();
		m_input_manager->initialize();


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
		Assimp_Config config;
		//Assimp_Model marry = Assimp_Model::load_model("C:/Users/30931/Desktop/Helios/Helios/engine/asset/model/Marry.obj", config);
		//Model marry = Model("C:/Users/30931/Desktop/Helios/Helios/engine/asset/model/Alisya/pink.pmx");
		//LOG_INFO("mesh: {0}, {1}", marry.meshes.size(), marry.meshes.size());

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
		m_pass = std::make_shared<OpenGL_GPU_Program>();
		m_pass->add_vertex_shader(vertex_shader);
		m_pass->add_fragment_shader(fragment_shader);
		m_pass->link_shader();


		context.m_main_camera->set_camera_properties(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));





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
		m_input_manager->process_control_command();
		context.m_imgui_layer->update();

		static glm::vec4 clear_color = glm::vec4(0.8f, 0.5f, 0.3f, 1.0f);
		static glm::vec3 model_pos = glm::vec3(0.0f);
		glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), model_pos);

		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Clear Color", glm::value_ptr(clear_color));
		ImGui::DragFloat3("model_pos", glm::value_ptr(model_pos), 0.01f);
		ImGui::End();

		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		m_pass->set_uniform("model_matrix", model_mat);
		m_pass->set_uniform("view_matrix", context.m_main_camera->get_view_matrix());
		m_pass->set_uniform("projection_matrix", context.m_main_camera->get_projection_matrix());

		context.m_imgui_layer->render();
		context.m_window->swap_buffers();
		context.m_window->poll_events();


	}
}
