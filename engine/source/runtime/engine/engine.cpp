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
			-1.0f,  1.0f,
			-1.0f, -1.0f,
			1.0f, -1.0f, 
			-1.0f,  1.0f,
			1.0f, -1.0f, 
			1.0f,  1.0f 
    	};

		float quadTexcoord[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 1.0f
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
		// screen quad VAO
		



		test_pass = std::make_unique<OpenGL_Pass>("test_pass");
		test_pass->vertex_shader = m_rhi->create_shader( "shader/bunny_vert.glsl");
		test_pass->fragment_shader = m_rhi->create_shader( "shader/bunny_frag.glsl");
		test_pass->shader_process();
		test_pass->set_uniform("skybox", 0);
		auto bunny_vertex_info = bunny.meshes[0].vertex_info;
		const int num_of_vertex = (int)(bunny_vertex_info.position.size());
		auto bunny_position_1 = std::span<glm::vec3>(bunny_vertex_info.position);
		auto bunny_normal_1 = std::span<glm::vec3>(bunny_vertex_info.normal);
		{
			RHI_Draw_Command cmd;
			cmd.vertex_array = m_rhi->create_vertex_array();
			auto& bunny_vertex_array = cmd.vertex_array;
			bunny_vertex_array->primitive_count += bunny_position_1.size() / (size_t)3;
			bunny_vertex_array->add_attributes({"POSITION", 3, bunny_position_1.size_bytes(), bunny_position_1.data()});
			bunny_vertex_array->add_attributes({"NORMAL", 3, bunny_normal_1.size_bytes(), bunny_normal_1.data()});
			bunny_vertex_array->create_buffer_and_set_data();
			test_pass->queue.emplace_back(std::move(cmd));
		}

		frame_buffer_pass = std::make_unique<OpenGL_Pass>("framebuffer_pass");
		frame_buffer_pass->vertex_shader = m_rhi->create_shader( "shader/framebuffer-vert.glsl");
		frame_buffer_pass->fragment_shader = m_rhi->create_shader( "shader/framebuffer-frag.glsl");
		frame_buffer_pass->shader_process();
		frame_buffer_pass->set_uniform("screenTexture", 0);
		{
			RHI_Draw_Command cmd;
			cmd.vertex_array = m_rhi->create_vertex_array();
			auto& quad_array = cmd.vertex_array;
			quad_array->primitive_count = 2;
			quad_array->add_attributes({"POSITION", 2, sizeof(quadVertices), &quadVertices});
			quad_array->add_attributes({"TEXCOORD", 2, sizeof(quadTexcoord), &quadTexcoord});
			quad_array->create_buffer_and_set_data();
			frame_buffer_pass->queue.emplace_back(std::move(cmd));
		}

		auto skybox_pass = std::make_unique<OpenGL_Pass>("skybox_pass");
		skybox_pass->vertex_shader = m_rhi->create_shader( "shader/skybox-vert.glsl");
		skybox_pass->fragment_shader = m_rhi->create_shader( "shader/skybox-frag.glsl");
		skybox_pass->shader_process();
		skybox_pass->set_uniform("skybox", 0);
		{
			RHI_Draw_Command cmd;
			cmd.vertex_array = m_rhi->create_vertex_array();
			auto& skybox_array = cmd.vertex_array;
			skybox_array->primitive_count = 12;
			skybox_array->add_attributes({"POSITION", 3, sizeof(skyboxVertices), &skyboxVertices});
			skybox_array->create_buffer_and_set_data();
			skybox_pass->queue.emplace_back(std::move(cmd));
		}

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
		GLuint b_index = glGetUniformBlockIndex(test_pass->gpu_program->id(), "transforms");
		glUniformBlockBinding(test_pass->gpu_program->id(), b_index, 0);
		unsigned int transform_ubo;
		glGenBuffers(1, &transform_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, transform_ubo);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, transform_ubo, 0, 2 * sizeof(glm::mat4));

		while (!context.m_window->should_close())
		{
			m_input_manager->process_control_command();
			context.m_imgui_layer->update();
			context.m_main_camera->update();
			glBindBuffer(GL_UNIFORM_BUFFER, transform_ubo);
			glm::mat4 view = context.m_main_camera->get_view_matrix();
			glm::mat4 proj = context.m_main_camera->get_projection_matrix();
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(proj));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
			// test_pass->set_uniform("view_matrix", context.m_main_camera->get_view_matrix());
			// test_pass->set_uniform("projection_matrix", context.m_main_camera->get_projection_matrix());
			test_pass->clear_state.clear_color = true;
			test_pass->clear_state.clear_color_value = clear_color;
			test_pass->clear_state.clear_depth = true;
			test_pass->update();
			test_pass->render();
			// draw skybox as last
			glEnable(GL_DEPTH_TEST);

        	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
			skybox_pass->gpu_program->bind();
			skybox_pass->set_uniform("view_matrix", glm::mat4(glm::mat3(context.m_main_camera->get_view_matrix())));
			skybox_pass->set_uniform("projection_matrix", context.m_main_camera->get_projection_matrix());
			skybox_pass->clear_state.allow_clear = false;
			skybox_pass->update();
        	// skybox cube
        	glActiveTexture(GL_TEXTURE0);
        	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        	skybox_pass->render();
        	glDepthFunc(GL_LESS);
			framebuffer->unbind();

			frame_buffer_pass->clear_state.clear_color = true;
			frame_buffer_pass->clear_state.clear_depth = false;
			frame_buffer_pass->gpu_program->bind();
			
			//frame_buffer_pass->update();
			frame_buffer_pass->update();
			frame_buffer_pass->enable_depth_test = false;
			glBindTexture(GL_TEXTURE_2D, framebuffer->texColorBuffer);
			frame_buffer_pass->render();


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
