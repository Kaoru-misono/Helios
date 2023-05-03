#include "opengl_rhi.hpp"
#include "logger/logger_marco.hpp"

namespace Helios
{
    OpenGL_RHI::~OpenGL_RHI()
    {

    }

    auto OpenGL_RHI::initialize(std::shared_ptr<Window>& window) -> void
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
}