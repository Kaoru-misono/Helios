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
		m_rhi->init(Window::instance());
		m_rhi->create_platform_context();
		LOG_INFO("Welcome to Helios !");
		m_rhi->init_imgui_for_platform();

		m_input_manager = std::make_shared<Input_Manager>();
		m_input_manager->initialize();

		std::vector<glm::vec2> quadVertices = {
			{-1.0f,  1.0f},
			{-1.0f, -1.0f},
			{1.0f, -1.0f},
			{-1.0f,  1.0f},
			{1.0f, -1.0f},
			{1.0f,  1.0f}
    	};

		std::vector<glm::vec2> quadTexcoord = {
			{0.0f, 1.0f},
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{0.0f, 1.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f}
    	};

		Assimp_Config config;
		Assimp_Model bunny = Assimp_Model::load_model("D:/github/Helios/engine/asset/model/bunny_1k.obj", config);
		Assimp_Model marry = Assimp_Model::load_model("D:/github/Helios/engine/asset/model/Alisya/pink.pmx", config);
		Assimp_Model cube = Assimp_Model::load_model("D:/github/Helios/engine/asset/model/cube.obj", config);
		auto cloth_tex = m_rhi->load_texture(Texture::Kind::TEX_2D, {"D:/github/Helios/engine/asset/model/Alisya/cloth.png"});
		auto cloth_tex_2 = m_rhi->load_texture(Texture::Kind::TEX_2D, {"D:/github/Helios/engine/asset/model/Alisya/cloth_2.png"});
		auto eye_tex = m_rhi->load_texture(Texture::Kind::TEX_2D, {"D:/github/Helios/engine/asset/model/Alisya/eye.png"});
		auto face_tex = m_rhi->load_texture(Texture::Kind::TEX_2D, {"D:/github/Helios/engine/asset/model/Alisya/face.png"});
		auto hair_tex = m_rhi->load_texture(Texture::Kind::TEX_2D, {"D:/github/Helios/engine/asset/model/Alisya/hair.png"});
		auto emote_tex = m_rhi->load_texture(Texture::Kind::TEX_2D, {"D:/github/Helios/engine/asset/model/Alisya/emote.png"});
		auto pink_tex = m_rhi->load_texture(Texture::Kind::TEX_2D, {"D:/github/Helios/engine/asset/texture/nk.png"});
		auto wall_tex = m_rhi->load_texture(Texture::Kind::TEX_2D, {"D:/github/Helios/engine/asset/texture/brickwall.jpg"});
		auto wall_normal_tex = m_rhi->load_texture(Texture::Kind::TEX_2D, {"D:/github/Helios/engine/asset/texture/brickwall_normal.jpg"});
		std::vector<std::string> faces
		{
			"D:/github/Helios/engine/asset/texture/sky-box/right.jpg",
			"D:/github/Helios/engine/asset/texture/sky-box/left.jpg",
			"D:/github/Helios/engine/asset/texture/sky-box/top.jpg",
			"D:/github/Helios/engine/asset/texture/sky-box/bottom.jpg",
			"D:/github/Helios/engine/asset/texture/sky-box/front.jpg",
			"D:/github/Helios/engine/asset/texture/sky-box/back.jpg"
		};
		auto cubemapTexture = m_rhi->load_texture(Texture::Kind::TEX_CUBE, faces);
		std::unordered_map<int, std::shared_ptr<Texture>> material_map{
			{0, cloth_tex},
			{1, cloth_tex},
			{2, cloth_tex},
			{3, cloth_tex},
			{4, cloth_tex},
			{5, cloth_tex},
			{6, cloth_tex},
			{7, cloth_tex},
			{8, cloth_tex},
			{9, cloth_tex},
			{10, cloth_tex_2},
			{11, cloth_tex_2},
			{12, cloth_tex_2},
			{13, cloth_tex_2},
			{14, hair_tex},
			{15, cloth_tex},
			{16, cloth_tex_2},
			{17, cloth_tex},
			{18, face_tex},
			{19, face_tex},
			{20, eye_tex},
			{21, emote_tex},
		};
		test_pass = std::make_unique<OpenGL_Pass>("test_pass");
		test_pass->vertex_shader = m_rhi->create_shader( "shader/alisya_vert.glsl");
		test_pass->fragment_shader = m_rhi->create_shader( "shader/alisya_frag.glsl");
		// Expode geometry shader, you need to replace fragment shader v -> g if you want to open it
		// test_pass->geometry_shader = m_rhi->create_shader( "shader/expode_geom.glsl");
		test_pass->shader_process();

		auto box_pass = std::make_shared<OpenGL_Pass>("box_pass");
		box_pass->vertex_shader = m_rhi->create_shader( "shader/simple_vert.glsl");
		box_pass->fragment_shader = m_rhi->create_shader( "shader/simple_frag.glsl");
		box_pass->shader_process();

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
			auto quad_vert = std::span<glm::vec2>(quadVertices);
			auto quad_texcoord = std::span<glm::vec2>(quadTexcoord);
			quad_array->add_attributes({"POSITION", quad_vert});
			quad_array->add_attributes({"TEXCOORD", quad_texcoord});
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
			auto box_vert = std::span<glm::vec3>(cube.meshes[0].vertex_info.position);
			skybox_array->add_attributes({"POSITION", box_vert});
			skybox_array->create_buffer_and_set_data();
			cmd.uniform.try_emplace("skybox", cubemapTexture);
			cmd.sampler.try_emplace("skybox", Texture_Sampler{});
			cmd.sampler["skybox"].warp_s = Texture_Sampler::Warp::clamp_to_edge;
			cmd.sampler["skybox"].warp_t = Texture_Sampler::Warp::clamp_to_edge;
			cmd.sampler["skybox"].warp_r = Texture_Sampler::Warp::clamp_to_edge;
			skybox_pass->queue.emplace_back(std::move(cmd));
		}

		context.m_main_camera->set_camera_parameters(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		//auto framebuffer = std::make_unique<OpenGL_Framebuffer>(glm::vec2{Window::instance().get_width(),Window::instance().get_height()});
		auto depth_texture = m_rhi->create_texture(Texture::Kind::TEX_2D, Texture::Format::depth24, 8192, 8192);
		auto framebuffer = m_rhi->create_framebuffer();
		framebuffer->depth.texture = depth_texture;
		framebuffer->attach();

		auto shadow_pass = std::make_shared<OpenGL_Pass>("shadow_pass");
		shadow_pass->clear_state.clear_color = false;
		shadow_pass->vertex_shader = m_rhi->create_shader( "shader/shadow_mapping_vert.glsl");
		shadow_pass->fragment_shader = m_rhi->create_shader( "shader/shadow_mapping_frag.glsl");
		shadow_pass->shader_process();
		{
			RHI_Draw_Command cmd;
			cmd.vertex_array = m_rhi->create_vertex_array();
			auto& vertex_array = cmd.vertex_array;
			vertex_array->primitive_count = 12;
			auto quad_vert = std::span<glm::vec2>(quadVertices);
			vertex_array->add_attributes({"POSITION", quad_vert});
			vertex_array->create_buffer_and_set_data();
			cmd.uniform.try_emplace("depth_map", depth_texture);
			Texture_Sampler sampler;
			sampler.min_filter = Texture_Sampler::Filter::nearest;
			sampler.mag_filter = Texture_Sampler::Filter::nearest;
			cmd.sampler.try_emplace("depth_map", sampler);
		}

		auto depth_debug_pass = std::make_unique<OpenGL_Pass>("depth_debug_pass");
		depth_debug_pass->vertex_shader = m_rhi->create_shader( "shader/debug_quad_vert.glsl");
		depth_debug_pass->fragment_shader = m_rhi->create_shader( "shader/debug_quad_frag.glsl");
		depth_debug_pass->shader_process();

		auto light_visualize_pass = std::make_unique<OpenGL_Pass>("light_visualize_pass");
		light_visualize_pass->clear_state.allow_clear = false;
		light_visualize_pass->vertex_shader = m_rhi->create_shader( "shader/light_vert.glsl");
		light_visualize_pass->fragment_shader = m_rhi->create_shader( "shader/light_frag.glsl");
		light_visualize_pass->shader_process();
		{
			RHI_Draw_Command cmd;
			cmd.vertex_array = m_rhi->create_vertex_array();
			auto& box_array = cmd.vertex_array;
			box_array->primitive_count = 12;
			auto box_vert = std::span<glm::vec3>(cube.meshes[0].vertex_info.position);
			box_array->add_attributes({"POSITION", box_vert});
			box_array->create_buffer_and_set_data();
			light_visualize_pass->queue.emplace_back(std::move(cmd));
		}

		// UBO
		GLuint b_index = glGetUniformBlockIndex(test_pass->gpu_program->id(), "transforms");
		glUniformBlockBinding(test_pass->gpu_program->id(), b_index, 0);
		unsigned int transform_ubo;
		glGenBuffers(1, &transform_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, transform_ubo);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, transform_ubo, 0, 2 * sizeof(glm::mat4));

		// Lighting
		glm::vec3 light_pos{0.0f, 1.5f, 2.0f};

		// Deferred shading
		auto g_buffer = m_rhi->create_framebuffer();
		int width =  Window::instance().get_width();
		int height =  Window::instance().get_height();

		auto position_buffer = m_rhi->create_texture(Texture::Kind::TEX_2D, Texture::Format::rgb16f, width, height);
		g_buffer->colors.emplace_back(position_buffer);
		auto normal_buffer = m_rhi->create_texture(Texture::Kind::TEX_2D, Texture::Format::rgb16f, width, height);
		g_buffer->colors.emplace_back(normal_buffer);
		auto albedo_spec_buffer = m_rhi->create_texture(Texture::Kind::TEX_2D, Texture::Format::rgba8, width, height);
		g_buffer->colors.emplace_back(albedo_spec_buffer);
		auto depth_buffer = m_rhi->create_texture(Texture::Kind::TEX_2D, Texture::Format::depth24, width, height);
		g_buffer->depth.texture = depth_buffer;
		g_buffer->attach();

		auto g_buffer_pass = std::make_shared<OpenGL_Pass>("g_buffer_pass");
		//TODO:


		while (!Window::instance().should_close())
		{
			m_input_manager->process_control_command();
			context.m_imgui_layer->update();
			context.m_main_camera->update();
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// UBO
			glBindBuffer(GL_UNIFORM_BUFFER, transform_ubo);
			glm::mat4 view = context.m_main_camera->get_view_matrix();
			glm::mat4 proj = context.m_main_camera->get_projection_matrix();
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(proj));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			//framebuffer->bind();
			glEnable(GL_DEPTH_TEST);

			static glm::vec4 clear_color = glm::vec4(0.8f, 0.5f, 0.3f, 1.0f);
			static glm::vec3 model_pos = glm::vec3(0.0f, -1.0f, 0.0f);
			static glm::vec3 box_pos = glm::vec3(0.0f, -1.1f, -0.6f);
			glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), model_pos);
			model_mat = glm::scale(model_mat, glm::vec3(0.1f));
			glm::mat4 box_model_mat = glm::translate(glm::mat4(1.0f), box_pos);
			box_model_mat = glm::scale(box_model_mat, glm::vec3(2.0f, 0.1f, 2.0f));
			glm::mat4 light_proj, light_view;
			glm::mat4 light_matrix;
			float near_plane = 0.1f, far_plane = 7.5f;
			light_proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
			light_view = glm::lookAt(light_pos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			light_matrix = light_proj * light_view;
			shadow_pass->set_uniform("light_matrix", light_matrix);
			shadow_pass->queue.clear();
			{
				for (auto& mesh: marry.meshes) {
					auto& vertex_info = mesh.vertex_info;
					auto position = std::span<glm::vec3>(vertex_info.position);
					RHI_Draw_Command cmd;
					cmd.vertex_array = m_rhi->create_vertex_array();
					auto& vertex_array = cmd.vertex_array;
					vertex_array->primitive_count += position.size() / (size_t)3;
					vertex_array->add_attributes({"POSITION", position});
					vertex_array->create_buffer_and_set_data();
					cmd.uniform.try_emplace("model_matrix", model_mat);
					shadow_pass->queue.emplace_back(cmd);
				}
				RHI_Draw_Command box_cmd;
				box_cmd.vertex_array = m_rhi->create_vertex_array();
				auto& vertex_array = box_cmd.vertex_array;
				vertex_array->primitive_count += 12;
				auto box_vert = std::span<glm::vec3>(cube.meshes[0].vertex_info.position);
				vertex_array->add_attributes({"POSITION", box_vert});
				vertex_array->create_buffer_and_set_data();
				box_cmd.uniform.try_emplace("model_matrix", box_model_mat);
				shadow_pass->queue.emplace_back(box_cmd);
			}
			glViewport(0, 0, 8192, 8192);
			framebuffer->bind();
			shadow_pass->update();
			shadow_pass->render();
			framebuffer->unbind();
			static bool enable_debug = false;
			static float shadow_bias = 0.01f;
			ImGui::Begin("Settings");
			ImGui::ColorEdit3("Clear Color", glm::value_ptr(clear_color));
			ImGui::DragFloat3("model_pos", glm::value_ptr(model_pos), 0.01f);
			ImGui::DragFloat3("box_pos", glm::value_ptr(box_pos), 0.01f);
			ImGui::DragFloat3("light_pos", glm::value_ptr(light_pos), 0.01f);
			ImGui::DragFloat("shadow_bias", &shadow_bias, 0.001f);
			ImGui::Checkbox("Debug: shadow map", &enable_debug);
			ImGui::End();

			{
				test_pass->queue.clear();
				int mesh_id = 0;
				for (auto& mesh: marry.meshes) {
					auto& vertex_info = mesh.vertex_info;
					const int num_of_vertex = (int)(vertex_info.position.size());
					auto position = std::span<glm::vec3>(vertex_info.position);
					auto normal = std::span<glm::vec3>(vertex_info.normal);
					auto texcoord = std::span<glm::vec2>(vertex_info.texcoord);
					RHI_Draw_Command cmd;
					cmd.vertex_array = m_rhi->create_vertex_array();
					auto& vertex_array = cmd.vertex_array;
					vertex_array->primitive_count += position.size() / (size_t)3;
					vertex_array->add_attributes({"POSITION", position});
					vertex_array->add_attributes({"NORMAL", normal});
					vertex_array->add_attributes({"TEXCOORD", texcoord});
					vertex_array->create_buffer_and_set_data();
					cmd.uniform["mesh_id"] = mesh_id;
					if (material_map[mesh_id] == nullptr)
						std::cout << mesh_id << std::endl;
					cmd.uniform.try_emplace("base_color", material_map[mesh_id]);
					cmd.sampler.try_emplace("base_color", Texture_Sampler{});
					cmd.uniform.try_emplace("shadow_map", depth_texture);
					Texture_Sampler sampler;
					sampler.min_filter = Texture_Sampler::Filter::nearest;
					sampler.mag_filter = Texture_Sampler::Filter::nearest;
					cmd.sampler.try_emplace("shadow_map", sampler);
					test_pass->queue.emplace_back(std::move(cmd));
					mesh_id++;
				}
			}
			test_pass->set_uniform("camera_pos", context.m_main_camera->get_position());
			test_pass->set_uniform("light_pos", light_pos);
			test_pass->set_uniform("model_matrix", model_mat);
			test_pass->set_uniform("light_matrix", light_matrix);
			test_pass->set_uniform("shadow_bias", shadow_bias);
			test_pass->set_uniform("time", (float)glfwGetTime());
			// test_pass->set_uniform("view_matrix", context.m_main_camera->get_view_matrix());
			// test_pass->set_uniform("projection_matrix", context.m_main_camera->get_projection_matrix());
			test_pass->clear_state.clear_color = true;
			test_pass->clear_state.clear_color_value = clear_color;
			test_pass->clear_state.clear_depth = true;
			test_pass->update();
			test_pass->render();

			{
				box_pass->queue.clear();
				RHI_Draw_Command box_cmd;
				box_cmd.vertex_array = m_rhi->create_vertex_array();
				auto& vertex_array = box_cmd.vertex_array;
				vertex_array->primitive_count += 12;
				auto box_vert = std::span<glm::vec3>(cube.meshes[0].vertex_info.position);
				auto box_texcoord = std::span<glm::vec2>(cube.meshes[0].vertex_info.texcoord);
				auto box_normal = std::span<glm::vec3>(cube.meshes[0].vertex_info.normal);
				auto box_tangent = std::span<glm::vec3>(cube.meshes[0].vertex_info.tangent);
				vertex_array->add_attributes({"POSITION", box_vert});
				vertex_array->add_attributes({"TEXCOORD", box_texcoord});
				vertex_array->add_attributes({"NORMAL", box_normal});
				vertex_array->add_attributes({"TANGENT", box_tangent});
				vertex_array->create_buffer_and_set_data();
				box_cmd.uniform.try_emplace("model_matrix", box_model_mat);
				box_cmd.uniform.try_emplace("shadow_map", depth_texture);
				Texture_Sampler sampler;
				sampler.min_filter = Texture_Sampler::Filter::nearest;
				sampler.mag_filter = Texture_Sampler::Filter::nearest;
				box_cmd.sampler.try_emplace("shadow_map", sampler);
				box_cmd.uniform.try_emplace("base_color", wall_tex);
				box_cmd.sampler.try_emplace("base_color", Texture_Sampler{});
				box_cmd.uniform.try_emplace("normal", wall_normal_tex);
				box_cmd.sampler.try_emplace("normal", Texture_Sampler{});
				box_pass->queue.emplace_back(std::move(box_cmd));
			}
			box_pass->set_uniform("view_matrix", context.m_main_camera->get_view_matrix());
			box_pass->set_uniform("projection_matrix", context.m_main_camera->get_projection_matrix());
			box_pass->set_uniform("light_matrix", light_matrix);
			box_pass->set_uniform("shadow_bias", shadow_bias);
			box_pass->clear_state.allow_clear = false;
			box_pass->update();
			box_pass->render();
			glm::mat4 light_box_mat = glm::translate(glm::mat4(1.0), light_pos);
			light_box_mat = glm::scale(light_box_mat, glm::vec3(0.05f));
			light_visualize_pass->set_uniform("model_matrix", light_box_mat);
			light_visualize_pass->set_uniform("view_matrix", context.m_main_camera->get_view_matrix());
			light_visualize_pass->set_uniform("projection_matrix", context.m_main_camera->get_projection_matrix());
			light_visualize_pass->update();
			light_visualize_pass->render();
			// draw skybox as last
			glEnable(GL_DEPTH_TEST);
        	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
			skybox_pass->set_uniform("view_matrix", glm::mat4(glm::mat3(context.m_main_camera->get_view_matrix())));
			skybox_pass->set_uniform("projection_matrix", context.m_main_camera->get_projection_matrix());
			skybox_pass->clear_state.allow_clear = false;
			skybox_pass->update();
        	// skybox cube
        	skybox_pass->render();
       		glDepthFunc(GL_LESS);


			{
				depth_debug_pass->queue.clear();
				RHI_Draw_Command cmd;
				cmd.vertex_array = m_rhi->create_vertex_array();
				auto& quad_array = cmd.vertex_array;
				quad_array->primitive_count = 2;
				auto quad_vert = std::span<glm::vec2>(quadVertices);
				auto quad_texcoord = std::span<glm::vec2>(quadTexcoord);
				quad_array->add_attributes({"POSITION", quad_vert});
				quad_array->add_attributes({"TEXCOORD", quad_texcoord});
				quad_array->create_buffer_and_set_data();
				cmd.uniform.try_emplace("near_plane", near_plane);
				cmd.uniform.try_emplace("far_plane", far_plane);
				cmd.uniform.try_emplace("depth_map", depth_texture);
				Texture_Sampler sampler;
				sampler.min_filter = Texture_Sampler::Filter::nearest;
				sampler.mag_filter = Texture_Sampler::Filter::nearest;
				cmd.sampler.try_emplace("depth_map", sampler);
				depth_debug_pass->queue.emplace_back(std::move(cmd));
			}
			if (enable_debug) {
				depth_debug_pass->update();
				depth_debug_pass->render();
			}
// 			framebuffer->unbind();
//
// 			frame_buffer_pass->clear_state.clear_color = true;
// 			frame_buffer_pass->clear_state.clear_depth = false;
// 			frame_buffer_pass->update();
// 			frame_buffer_pass->enable_depth_test = false;
// 			glBindTexture(GL_TEXTURE_2D, framebuffer->texColorBuffer);
// 			frame_buffer_pass->render();


			context.m_imgui_layer->render();
			Window::instance().swap_buffers();
			Window::instance().poll_events();
		}
	}

	auto Helios_Engine::shutdown() -> void
	{
		context.shutdown_context();
	}

	auto Helios_Engine::renderer_tick() -> void
	{

	}
}
