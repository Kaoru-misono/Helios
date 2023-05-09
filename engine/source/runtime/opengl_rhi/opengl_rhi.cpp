#include "opengl_rhi.hpp"
#include "logger/logger_marco.hpp"
#include "opengl_buffer.hpp"
#include "opengl_vertex_array.hpp"
#include "opengl_shader.hpp"
#include <fstream>

namespace Helios
{
    OpenGL_RHI::~OpenGL_RHI()
    {

    }

    auto OpenGL_RHI::init(std::shared_ptr<Window>& window) -> void
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window_ = window->get_window();
    }

    auto OpenGL_RHI::create_context() -> void
    {
        glfwMakeContextCurrent(window_);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_ERROR("Failed to initialized Glad!");
			return;
		}

        LOG_INFO("OpenGL Info:");
		LOG_INFO("Vendor : {0}", glGetString(GL_VENDOR));
		LOG_INFO("Renderer : {0}", glGetString(GL_RENDERER));
		LOG_INFO("Version : {0}", glGetString(GL_VERSION));

    }

    auto OpenGL_RHI::create_buffer(RHI_Buffer_Create_info& buffer_create_info, RHI_Usage_Flag flag, uint32_t size, uint32_t stride) -> std::shared_ptr<RHI_Buffer>
    {
        if(buffer_create_info.data_array->size == 0)
        {
            //TODO: when size == 0, use create_info.data_array.size
            LOG_ERROR("Create info has no data!");
            return nullptr;
        }
        
        GLenum buffer_type = GL_ARRAY_BUFFER;
        if(flag == RHI_Usage_Flag::index_buffer)
        buffer_type = GL_ELEMENT_ARRAY_BUFFER;

        auto& data_array = buffer_create_info.data_array;

        std::shared_ptr<OpenGL_Buffer> buffer = std::make_shared<OpenGL_Buffer>(buffer_type, flag, size, data_array);
        
        return buffer;
    }

    auto OpenGL_RHI::create_vertex_array() -> std::shared_ptr<RHI_Vertex_Array>
    {
        std::shared_ptr<RHI_Vertex_Array> vertex_array = std::make_shared<OpenGL_Vertex_Array>();
        return vertex_array;
    }

    auto OpenGL_RHI::create_shader(const std::string& vert_path, const std::string& frag_path) -> std::shared_ptr<RHI_Shader>
    {
        std::shared_ptr<RHI_Shader> shader = std::make_shared<OpenGL_Shader>(vert_path, frag_path);
        return shader;
    }

    //auto OpenGL_RHI::read_file(const std::string& path, Shader_Type& type) -> std::string
    //{
    //    auto idx = path.find_last_of(".");
	//	auto sub_str = path.substr(idx-4, 4);
	//	LOG_WARN(sub_str);
	//	if (sub_str == "vert")
	//		type = Shader_Type::VERTEX_SHADER;
	//	else if (sub_str == "frag")
	//		type = Shader_Type::FRAGMENT_SHADER;
	//	else
	//		LOG_ERROR("shader path name is illegal !");
	//    std::string ShaderSrc;
	//    std::ifstream in(path, std::ios::in, std::ios::binary);
	//    if (in)
	//    {
	//    	in.seekg(0, std::ios::end);
	//    	size_t size = in.tellg();
	//    	if (size != -1)
	//    	{
	//    		ShaderSrc.resize(size);
	//    		in.seekg(0, std::ios::beg);
	//    		in.read(&ShaderSrc[0], size);
	//    		in.close();
	//    	}
	//    	else
	//    		LOG_ERROR( "Could not read from file ", path );
	//    }
	//    else
	//    	LOG_ERROR( "Could not open the file! " );
	//    return ShaderSrc;
    //}

   
}