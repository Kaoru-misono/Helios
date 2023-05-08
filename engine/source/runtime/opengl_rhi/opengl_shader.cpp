#include "opengl_shader.hpp"
#include "logger/logger_marco.hpp"
#include <fstream>

namespace Helios
{
    OpenGL_Shader::OpenGL_Shader(const std::string& in_vert_path, const std::string& in_frag_path)
    {
		resource = glCreateProgram();
		std::string vert_src = read_file(in_vert_path);
		std::string frag_src = read_file(in_frag_path);
        compile(vert_src, frag_src);
    }

    OpenGL_Shader::~OpenGL_Shader()
    {
        glDeleteProgram(resource);
    }

    auto OpenGL_Shader::bind() ->void
    {
        glUseProgram(resource);
    }

    auto OpenGL_Shader::set_uniform() ->void
    {

    }

    auto OpenGL_Shader::read_file(const std::string& path) -> std::string
    {
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
	    		LOG_ERROR( "Could not read from file ", path );
	    }
	    else
	    	LOG_ERROR( "Could not open the file! " );
	    return ShaderSrc;
    }

    auto OpenGL_Shader::compile(const std::string& vert_src, const std::string& frag_src) -> void
    {
        //--------------Create and Compile Shader-----------------------
	    unsigned int vertex_shader, fragment_shader;

	    // Create an empty vertex shader handle
	    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	    // Send the vertex shader source code to GL
	    // Note that std::string's .c_str is NULL character terminated.
	    const GLchar* source = vert_src.c_str();
	    glShaderSource(vertex_shader, 1, &source, 0);
	    // Compile the vertex shader
	    glCompileShader(vertex_shader);

	    GLint is_compiled = 0;
	    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
	    if (is_compiled == GL_FALSE)
	    {
	    	GLint max_length = 0;
	    	glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

	    	// The max_length includes the NULL character
	    	std::vector<GLchar> infoLog(max_length);
	    	glGetShaderInfoLog(vertex_shader, max_length, &max_length, &infoLog[0]);

	    	// We don't need the shader anymore.
	    	glDeleteShader(vertex_shader);

	    	// Use the infoLog as you see fit.

	    	// In this simple program, we'll just leave
	    	LOG_ERROR( infoLog.data() );
	    	LOG_ERROR( "Vertex Shader Compilation failed!" );
	    	return;
	    }

	    // Create an empty Fragment shader handle
	    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	    // Send the Fragment shader source code to GL
	    // Note that std::string's .c_str is NULL character terminated.
	    source = frag_src.c_str();
	    glShaderSource(fragment_shader, 1, &source, 0);
	    // Compile the Fragment shader
	    glCompileShader(fragment_shader);

	    is_compiled = 0;
	    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
	    if (is_compiled == GL_FALSE)
	    {
	    	GLint max_length = 0;
	    	glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

	    	// The max_length includes the NULL character
	    	std::vector<GLchar> infoLog(max_length);
	    	glGetShaderInfoLog(fragment_shader, max_length, &max_length, &infoLog[0]);

	    	// We don't need the shader anymore.
	    	glDeleteShader(fragment_shader);

	    	// Use the infoLog as you see fit.

	    	// In this simple program, we'll just leave
	    	LOG_ERROR( infoLog.data() );
	    	LOG_ERROR( "Fragment Shader Compilation failed!" );
	    	return;
	    }

	    glAttachShader(resource, vertex_shader);
	    glAttachShader(resource, fragment_shader);
	    glLinkProgram(resource);

	    // Note the different functions here: glGetProgram* instead of glGetShader*.
	    GLint isLinked = 0;
	    glGetProgramiv(resource, GL_LINK_STATUS, (int*)&isLinked);
	    if (isLinked == GL_FALSE)
	    {
	    	GLint maxLength = 0;
	    	glGetProgramiv(resource, GL_INFO_LOG_LENGTH, &maxLength);

	    	// The max_length includes the NULL character
	    	std::vector<GLchar> infoLog(maxLength);
	    	glGetProgramInfoLog(resource, maxLength, &maxLength, &infoLog[0]);

	    	// We don't need the program anymore.
	    	glDeleteProgram(resource);
	    	// Don't leak shaders either.
	    	glDeleteShader(vertex_shader);
	    	glDeleteShader(fragment_shader);

	    	// Use the infoLog as you see fit.

	    	// In this simple program, we'll just leave
	    	LOG_ERROR( infoLog.data() );
	    	LOG_ERROR( "Shader link failed!" );

	    	return;
        }

        glDetachShader(resource, vertex_shader);
	    glDetachShader(resource, fragment_shader);

	    glDeleteShader(vertex_shader);
	    glDeleteShader(fragment_shader);
    }

}