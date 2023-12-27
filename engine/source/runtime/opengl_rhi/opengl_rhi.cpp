#include "opengl_rhi.hpp"
#include "logger/logger_marco.hpp"
#include "opengl_buffer.hpp"
#include "opengl_vertex_array.hpp"
#include "opengl_shader.hpp"
#include "opengl_texture.hpp"
#include "opengl_framebuffer.hpp"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <fstream>
#include <filesystem>

namespace Helios
{
    namespace
    {
        std::string bin_dir = std::filesystem::current_path().string();

        auto get_source_dir = [](std::string const& bin_dir) -> std::string
        {
            auto ps = bin_dir.rfind("Helios");

			auto source_path = bin_dir.substr(0, ps);

            replace(source_path.begin(), source_path.end(), '\\', '/');
			return source_path;
        };

        auto source_dir = get_source_dir(bin_dir);

        auto asset_dir = source_dir + "Helios/engine/asset/";
    }

    OpenGL_RHI::~OpenGL_RHI()
    {

    }

    auto OpenGL_RHI::init(Window& window) -> void
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window_ = window.get_window();
    }

    auto OpenGL_RHI::create_platform_context() -> void
    {
        glfwMakeContextCurrent(window_);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_ERROR("Failed to initialized Glad!");
			return;
		}

        LOG_INFO("OpenGL Info:");
		LOG_INFO("Support : {0}", glGetString(GL_VENDOR));
		LOG_INFO("GPU : {0}", glGetString(GL_RENDERER));
		LOG_INFO("Version : {0}", glGetString(GL_VERSION));

    }

    auto OpenGL_RHI::init_imgui_for_platform() -> void
    {
        // Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window_, true);
		ImGui_ImplOpenGL3_Init("#version 460");
    }

    auto OpenGL_RHI::create_buffer(RHI_Buffer_Create_info& buffer_create_info, RHI_Usage_Flag flag) -> std::shared_ptr<RHI_Buffer>
    {
        if(buffer_create_info.data_array.empty())
        {
            //TODO: when size == 0, use create_info.data_array.size
            LOG_ERROR("Create info has no data!");
            return nullptr;
        }

        GLenum buffer_type = GL_ARRAY_BUFFER;
        if(flag == RHI_Usage_Flag::index_buffer)
        buffer_type = GL_ELEMENT_ARRAY_BUFFER;

        auto& data_array = buffer_create_info.data_array;
        std::shared_ptr<OpenGL_Buffer> buffer;

        if (data_array.size() == 1) {
            auto size = data_array[0].size;
            //std::cout << size << std::endl;

            buffer = std::make_shared<OpenGL_Buffer>(buffer_type, flag, size, data_array[0]);
        }
        else {
            auto size = 0;
            for (auto data: data_array) {
                size += data.size;
            }
            //std::cout << size << std::endl;
            buffer = std::make_shared<OpenGL_Buffer>(buffer_type, flag, size, data_array);
        }


        return buffer;
    }

    auto OpenGL_RHI::create_vertex_array() -> std::shared_ptr<RHI_Vertex_Array>
    {
        std::shared_ptr<RHI_Vertex_Array> vertex_array = std::make_shared<OpenGL_Vertex_Array>();
        return vertex_array;
    }

    auto OpenGL_RHI::create_shader(const std::string& path) -> std::shared_ptr<RHI_Shader>
    {
        std::shared_ptr<RHI_Shader> shader = std::make_shared<OpenGL_Shader>(asset_dir + path);
        return shader;
    }

    auto OpenGL_RHI::create_texture(Texture::Kind kind, std::vector<std::string> const& paths, Texture::Format format, bool should_flip) -> std::shared_ptr<Texture>
    {
        std::shared_ptr<Texture> texture;
        if (kind == Texture::Kind::TEX_2D && paths.size() == 1)
            texture = OpenGL_Texture::load_2D_texture(paths[0], should_flip);
        else if (kind == Texture::Kind::TEX_2D && format == Texture::Format::depth24)
            texture = OpenGL_Texture::create_depth_map_texture();
        else if (kind == Texture::Kind::TEX_CUBE)
            texture = OpenGL_Texture::load_cube_map_texture(paths, should_flip);
        else
            LOG_INFO("Texture create failed.");
        return texture;
    }

    auto OpenGL_RHI::create_framebuffer() -> std::shared_ptr<RHI_Framebuffer>
    {
        auto framebuffer = std::make_shared<OpenGL_Framebuffer>();
        return framebuffer;
    }
}
