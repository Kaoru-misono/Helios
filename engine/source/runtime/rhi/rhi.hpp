#pragma once
#include <memory>
#include "window/window.hpp"
#include "buffer.hpp"
#include "rhi/rhi_defination.hpp"
#include "rhi/vertex_array_spcifier.hpp"
#include "rhi/texture.hpp"
namespace Helios
{
    class RHI
    {
    public:
        RHI() = default;
        //you must define Destructor here instead of pure function, otherwise you will get link error
        virtual ~RHI() {};
        virtual auto init(Window& window) -> void = 0;
        virtual auto create_platform_context() -> void = 0;
        virtual auto init_imgui_for_platform() -> void = 0;
        virtual auto create_buffer(RHI_Buffer_Create_info& buffer_create_info, RHI_Usage_Flag flag) -> std::shared_ptr<RHI_Buffer> = 0;
        virtual auto create_vertex_array() -> std::shared_ptr<RHI_Vertex_Array> = 0;
        virtual auto create_shader(const std::string& path) -> std::shared_ptr<RHI_Shader> = 0;
        virtual auto create_texture(Texture::Kind kind, std::vector<std::string> const& paths) -> std::shared_ptr<Texture> = 0;
        //virtual auto
    private:
    };

}
