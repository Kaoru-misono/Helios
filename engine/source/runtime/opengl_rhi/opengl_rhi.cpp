#include "opengl_rhi.hpp"
#include "logger/logger_marco.hpp"
#include "opengl_buffer.hpp"

namespace Helios
{
    OpenGL_RHI::~OpenGL_RHI()
    {

    }

    auto OpenGL_RHI::init(std::shared_ptr<Window>& window) -> void
    {
        _window = window->get_window();
    }

    auto OpenGL_RHI::create_context() -> void
    {
        glfwMakeContextCurrent(_window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_ERROR("Failed to initialized Glad!");
			return;
		}
    }

    auto OpenGL_RHI::create_buffer(RHI_Buffer_Create_info& buffer_create_info, RHI_Usage_Flag flag, uint32_t size, uint32_t stride) -> std::shared_ptr<RHI_Buffer>
    {
        if(buffer_create_info.data_array->size == 0)
        {
            LOG_ERROR("Create info has no data!");
            return nullptr;
        }
        
        GLenum buffer_type = GL_ARRAY_BUFFER;
        if(flag == RHI_Usage_Flag::index_buffer)
        buffer_type = GL_ELEMENT_ARRAY_BUFFER;

        auto& data_array = buffer_create_info.data_array;

        std::shared_ptr<OpenGL_Buffer> buffer = std::make_shared<OpenGL_Buffer>(buffer_type, flag, size, data_array);
        
        return std::make_shared<RHI_Buffer>();
    }
}