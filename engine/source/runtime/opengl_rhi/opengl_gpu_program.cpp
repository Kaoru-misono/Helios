#include "opengl_gpu_program.hpp"
#include "logger/logger_marco.hpp"
#include "opengl_shader.hpp"

namespace Helios
{
    OpenGL_GPU_Program::OpenGL_GPU_Program()
    {
        uniforms.clear();
        resource = glCreateProgram();
        glUseProgram(resource);
    }

    OpenGL_GPU_Program::~OpenGL_GPU_Program()
    {
        glDeleteProgram(resource);
    }

    auto OpenGL_GPU_Program::bind() -> void
    {
        glUseProgram(resource);
    }


    auto OpenGL_GPU_Program::add_vertex_shader(const std::shared_ptr<RHI_Shader>& vertex_shader) -> void
    {
        //vertex_shader_ = OpenGL_Shader::create_gl_shader(vertex_shader->get_shader_source(), vertex_shader->get_shader_type());
        //OpenGL_Shader::compile(vertex_shader_, vertex_shader->get_shader_type());
        vertex_shader_ = std::move(vertex_shader);
    }

    auto OpenGL_GPU_Program::add_fragment_shader(const std::shared_ptr<RHI_Shader>& fragment_shader) -> void
    {
        //fragment_shader_ = OpenGL_Shader::create_gl_shader(fragment_shader->get_shader_source(), fragment_shader->get_shader_type());
        //OpenGL_Shader::compile(fragment_shader_, fragment_shader->get_shader_type());
        fragment_shader_ = std::move(fragment_shader);
    }

    auto OpenGL_GPU_Program::link_shader() -> void
    {
        attach_shader(vertex_shader_);
        attach_shader(fragment_shader_);
        link();
        detach_shader(vertex_shader_);
        detach_shader(fragment_shader_);
        bind();
    }


    auto OpenGL_GPU_Program::attach_shader(const std::shared_ptr<RHI_Shader>& shader) -> void
    {
        glAttachShader(resource, shader->get_shader_id());
    }

    auto OpenGL_GPU_Program::detach_shader(const std::shared_ptr<RHI_Shader>& shader) -> void
    {
        glDetachShader(resource, shader->get_shader_id());
    }

    auto OpenGL_GPU_Program::link() ->void
    {
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

	    	// In this simple program, we'll just leave
	    	LOG_ERROR( infoLog.data() );
	    	LOG_ERROR( "Shader link failed!" );

	    	return;
        }

    }

    auto OpenGL_GPU_Program::set_uniform(const std::string& name, std::any const& value) -> void
    {
        if(!value.has_value())
        return;

        auto idx = uniforms.find(name);
        if (idx != uniforms.end())
        {
            LOG_WARN("Uniform:[ {0} ]has exists!", name);
            return;
        }
        else
            uniforms.emplace(name);

        bind(); // bind before you set uniform

        const std::string type = value.type().name();
        if      (type == "int")
            set_uniform_impl(name, std::any_cast<int>(value));
        else if (type == "float")
            set_uniform_impl(name, std::any_cast<float>(value));
        else if (type == "struct glm::vec<2,float,0>")
            set_uniform_impl(name, std::any_cast<glm::vec2>(value));
        else if (type == "struct glm::vec<3,float,0>")
            set_uniform_impl(name, std::any_cast<glm::vec3>(value));
        else if (type == "struct glm::vec<4,float,0>")
            set_uniform_impl(name, std::any_cast<glm::vec4>(value));
        else if (type == "struct glm::mat<3,3,float,0>")
            set_uniform_impl(name, std::any_cast<glm::mat3>(value));
        else if (type == "struct glm::mat<4,4,float,0>")
            set_uniform_impl(name, std::any_cast<glm::mat4>(value));

    }
}