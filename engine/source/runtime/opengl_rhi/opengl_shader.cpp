#include "opengl_shader.hpp"
#include "logger/logger_marco.hpp"
#include <fstream>

namespace Helios
{
    OpenGL_Shader::OpenGL_Shader(const std::string& in_path): name(in_path)
    {
		shader_source_ = read_file(in_path, type_);
		shader_id_ = create_gl_shader(shader_source_, type_);
		compile(shader_id_, type_);
    }

    OpenGL_Shader::~OpenGL_Shader()
    {
		glDeleteShader(shader_id_);
    }

    auto OpenGL_Shader::read_file(const std::string& path, Shader_Type& type) -> std::string
    {
		auto idx = path.find_last_of(".");
		auto sub_str = path.substr(idx - 4, 4);
		if (sub_str == "vert")
			type = Shader_Type::VERTEX_SHADER;
		else if (sub_str == "frag")
			type = Shader_Type::FRAGMENT_SHADER;
		else if (sub_str == "geom")
			type = Shader_Type::GEOMETRY_SHADER;
		else
			LOG_ERROR("shader path name is illegal !");
	    std::string ShaderSrc;
	    std::ifstream in(path, std::ios::in, std::ios::binary);
	    if (in)
	    {
	    	in.seekg(0, std::ios::end);
	    	size_t size = in.tellg();
	    	if (size != -1)
	    	{
	    		ShaderSrc.resize(size);
	    		in.seekg(0, std::ios::beg);
	    		in.read(&ShaderSrc[0], size);
	    		in.close();
	    	}
	    	else
	    		LOG_ERROR( "Could not read from file: {0}", path );
	    }
	    else
	    	LOG_ERROR( "Could not open the file: {0}", path);
	    return ShaderSrc;
    }

	auto OpenGL_Shader::create_gl_shader(const std::string& src, Shader_Type type) -> unsigned int
	{
		//--------------Create and Compile Shader-----------------------
		unsigned int shader;
	    // Create an empty vertex shader handle
		if(type == Shader_Type::VERTEX_SHADER)
	    	shader = glCreateShader(GL_VERTEX_SHADER);
		else if(type == Shader_Type::FRAGMENT_SHADER)
	    	shader = glCreateShader(GL_FRAGMENT_SHADER);
		else if (type == Shader_Type::GEOMETRY_SHADER)
			shader = glCreateShader(GL_GEOMETRY_SHADER);

	    // Send the vertex shader source code to GL
	    // Note that std::string's .c_str is NULL character terminated.
	    const GLchar* source = src.c_str();
	    glShaderSource(shader, 1, &source, 0);

		std::string shader_type = type == Shader_Type::VERTEX_SHADER ? "Vertex shader" : "Fragment shader";
		//LOG_TRACE(shader_type + " created succeed!");
		return shader;
	}

    auto OpenGL_Shader::compile(unsigned int shader, Shader_Type type) -> void
    {
	    // Compile the shader
	    glCompileShader(shader);

		auto shader_type =
			type == Shader_Type::VERTEX_SHADER ? "Vertex shader" :
			type == Shader_Type::FRAGMENT_SHADER ? "Fragment shader" :
			"Geometry shader";
	    GLint is_compiled = 0;
	    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
	    if (is_compiled == GL_FALSE)
	    {
	    	GLint max_length = 0;
	    	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

	    	// The max_length includes the NULL character
	    	std::vector<GLchar> infoLog(max_length);
	    	glGetShaderInfoLog(shader, max_length, &max_length, &infoLog[0]);

	    	// We don't need the shader anymore.
	    	glDeleteShader(shader);

	    	// Use the infoLog as you see fit.
	    	// In this simple program, we'll just leave
	    	LOG_ERROR( infoLog.data() );
	    	LOG_ERROR("{0} Compilation failed ! ", shader_type);
	    	return;
	    }
		//LOG_TRACE(shader_type + " compile succeed!");
    }

	auto OpenGL_Shader::shader_id()const -> unsigned int
	{
		return shader_id_;
	}


	auto OpenGL_Shader::shader_source()const -> std::string
	{
		return shader_source_;
	}

    auto OpenGL_Shader::shader_type()const -> Shader_Type
	{
		return type_;
	}
}
