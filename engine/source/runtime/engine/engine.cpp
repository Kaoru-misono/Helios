#include "engine.hpp"
#include <iostream>
#include <memory>
#include <any>
#include <glm/glm.hpp>
#include <filesystem>
#include <imgui.h>
#include <span>
#include <random>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/gtc/type_ptr.hpp>

#include "logger/logger.hpp"
#include "logger/logger_marco.hpp"
#include "window/window.hpp"
#include "opengl_rhi/opengl_rhi.hpp"
#include "rhi/rhi_defination.hpp"
#include "opengl_rhi/opengl_gpu_program.hpp"
#include "opengl_rhi/opengl_pass.hpp"
#include "opengl_rhi/opengl_draw_command.hpp"
#include "opengl_rhi/opengl_framebuffer.hpp"
#include "render/camera.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <q_model.hpp>
#include <stb_image.h>
#include "import_assimp_model/import_assimp_model.hpp"


namespace Helios
{
	static std::uniform_real_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;

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
		m_rhi->init(*context.m_window);
		m_rhi->create_platform_context();
		LOG_INFO("Welcome to Helios !");
		m_rhi->init_imgui_for_platform();

		m_input_manager = std::make_shared<Input_Manager>();
		m_input_manager->initialize();

		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
    	};

		float skyboxVertices[] = {
			// positions
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};


		Assimp_Config config;
		Assimp_Model bunny = Assimp_Model::load_model("D:/github/Helios/engine/asset/model/bunny_1k.obj", config);
		//Assimp_Model marry = Model("C:/Users/30931/Desktop/Helios/Helios/engine/asset/model/Alisya/pink.pmx");

		auto bunny_vertex_info = bunny.meshes[0].vertex_info;
		const int num_of_vertex = (int)(bunny_vertex_info.position.size());
		//std::vector<glm::vec3> bunny_vertices;
		auto bunny_position_1 = std::span<glm::vec3>(bunny_vertex_info.position);
		auto bunny_normal_1 = std::span<glm::vec3>(bunny_vertex_info.normal);
		auto bunny_vertex_array = m_rhi->create_vertex_array();
		bunny_vertex_array->add_attributes({"POSITION", 3, bunny_position_1.size_bytes(), bunny_position_1.data()});
		bunny_vertex_array->add_attributes({"NORMAL", 3, bunny_normal_1.size_bytes(), bunny_normal_1.data()});
		bunny_vertex_array->create_buffer_and_set_data();
		bunny_vertex_array->set_attributes_pointer();
		// screen quad VAO
    	unsigned int quadVAO, quadVBO;
    	glGenVertexArrays(1, &quadVAO);
    	glGenBuffers(1, &quadVBO);
    	glBindVertexArray(quadVAO);
    	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    	glEnableVertexAttribArray(0);
    	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    	glEnableVertexAttribArray(1);
    	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		unsigned int skyboxVAO, skyboxVBO;
    	glGenVertexArrays(1, &skyboxVAO);
    	glGenBuffers(1, &skyboxVBO);
    	glBindVertexArray(skyboxVAO);
    	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    	glEnableVertexAttribArray(0);
    	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		// RHI_Buffer_Create_info index_info;
		// index_info.data_array = std::make_shared<Data_Array>(sizeof(indices), indices);
		// std::shared_ptr<RHI_Buffer> index_buffer = m_rhi->create_buffer(index_info, RHI_Usage_Flag::index_buffer, index_info.data_array->size, 0);

		test_pass = std::make_unique<OpenGL_Pass>("test_pass");
		test_pass->vertex_shader = m_rhi->create_shader( "shader/bunny_vert.glsl");
		test_pass->fragment_shader = m_rhi->create_shader( "shader/bunny_frag.glsl");
		test_pass->shader_process();
		test_pass->uniforms["skybox"] = 0;

		frame_buffer_pass = std::make_unique<OpenGL_Pass>("framebuffer_pass");
		frame_buffer_pass->vertex_shader = m_rhi->create_shader( "shader/framebuffer-vert.glsl");
		frame_buffer_pass->fragment_shader = m_rhi->create_shader( "shader/framebuffer-frag.glsl");
		frame_buffer_pass->shader_process();
		frame_buffer_pass->uniforms["screenTexture"] = 0;

		auto skybox_pass = std::make_unique<OpenGL_Pass>("skybox_pass");
		skybox_pass->vertex_shader = m_rhi->create_shader( "shader/skybox-vert.glsl");
		skybox_pass->fragment_shader = m_rhi->create_shader( "shader/skybox-frag.glsl");
		skybox_pass->shader_process();
		skybox_pass->uniforms["skybox"] = 0;

		context.m_main_camera->set_camera_parameters(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		auto framebuffer = std::make_unique<OpenGL_Framebuffer>(glm::vec2{context.m_window->get_width(), context.m_window->get_height()});

		auto loadCubemap = [](std::vector<std::string> faces) -> unsigned int
		{
			unsigned int textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

			int width, height, nrChannels;
			for (unsigned int i = 0; i < faces.size(); i++)
			{
				unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
				if (data)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
								0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
					);
					stbi_image_free(data);
				}
				else
				{
					std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
					stbi_image_free(data);
				}
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			return textureID;
		};

		vector<std::string> faces
		{
			"D:/github/Helios/engine/asset/texture/sky-box/right.jpg",
			"D:/github/Helios/engine/asset/texture/sky-box/left.jpg",
			"D:/github/Helios/engine/asset/texture/sky-box/top.jpg",
			"D:/github/Helios/engine/asset/texture/sky-box/bottom.jpg",
			"D:/github/Helios/engine/asset/texture/sky-box/front.jpg",
			"D:/github/Helios/engine/asset/texture/sky-box/back.jpg"
		};
		unsigned int cubemapTexture = loadCubemap(faces);

		while (!context.m_window->should_close())
		{
			m_input_manager->process_control_command();
			context.m_imgui_layer->update();
			context.m_main_camera->update();
			framebuffer->bind();
			glActiveTexture(GL_TEXTURE0);
        	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			renderer_tick();
			glEnable(GL_DEPTH_TEST);

			static glm::vec4 clear_color = glm::vec4(0.8f, 0.5f, 0.3f, 1.0f);
			static glm::vec3 model_pos = glm::vec3(0.0f);
			glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), model_pos);

			ImGui::Begin("Settings");
			ImGui::ColorEdit3("Clear Color", glm::value_ptr(clear_color));
			ImGui::DragFloat3("model_pos", glm::value_ptr(model_pos), 0.01f);
			ImGui::End();

			test_pass->set_uniform("camera_pos", context.m_main_camera->get_position());
			test_pass->set_uniform("model_matrix", model_mat);
			test_pass->set_uniform("view_matrix", context.m_main_camera->get_view_matrix());
			test_pass->set_uniform("projection_matrix", context.m_main_camera->get_projection_matrix());
			test_pass->clear_state.clear_color = true;
			test_pass->clear_state.clear_color_value = clear_color;
			test_pass->clear_state.clear_depth = true;
			test_pass->update();
			bunny_vertex_array->bind();
			//glBindVertexArray(bunny_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3000);
			// draw skybox as last
			glEnable(GL_DEPTH_TEST);

        	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
			skybox_pass->gpu_program->bind();
			skybox_pass->set_uniform("view_matrix", glm::mat4(glm::mat3(context.m_main_camera->get_view_matrix())));
			skybox_pass->set_uniform("projection_matrix", context.m_main_camera->get_projection_matrix());
			skybox_pass->clear_state.allow_clear = false;
			skybox_pass->update();
        	// skybox cube
        	glBindVertexArray(skyboxVAO);
        	glActiveTexture(GL_TEXTURE0);
        	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        	glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
        	glDepthFunc(GL_LESS);
			framebuffer->unbind();

			frame_buffer_pass->clear_state.clear_color = true;
			frame_buffer_pass->clear_state.clear_depth = false;
			frame_buffer_pass->gpu_program->bind();
			glBindVertexArray(quadVAO);
			glDisable(GL_DEPTH_TEST);
			glBindTexture(GL_TEXTURE_2D, framebuffer->texColorBuffer);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			context.m_imgui_layer->render();
			context.m_window->swap_buffers();
			context.m_window->poll_events();
		}
	}

	auto Helios_Engine::shutdown() -> void
	{
		context.shutdown_context();
	}

	auto Helios_Engine::renderer_tick() -> void
	{
		
		//test_pass->render();
		//glDrawElements(GL_TRIANGLES, 3000, GL_UNSIGNED_INT, 0);
	}
}
