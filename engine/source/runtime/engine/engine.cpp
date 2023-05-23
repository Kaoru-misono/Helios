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
		m_rhi->create_context();
		LOG_INFO("Welcome to Helios !");

		{
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			GLFWwindow* window = static_cast<GLFWwindow*>(context.m_window->get_window());

			// Setup Platform/Renderer backends
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 460");
		}

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
		Assimp_Model marry = Assimp_Model::load_model("C:/Users/30931/Desktop/Helios/Helios/engine/asset/model/Marry.obj", config);
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
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		context.shutdown_context();
	}

	auto Helios_Engine::renderer_tick() -> void
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		static glm::vec4 clear_color = glm::vec4(0.8f, 0.5f, 0.3f, 1.0f);
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Flat Color", glm::value_ptr(clear_color));

		ImGui::End();
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)context.m_window->get_width(), (float)context.m_window->get_height());

		//Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);

		}

		context.m_window->swap_buffers();
		context.m_window->poll_events();

		
	}
}
