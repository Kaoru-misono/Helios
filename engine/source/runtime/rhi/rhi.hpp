#pragma once
#include <memory>
#include "window/window.hpp"
#include "rhi/rhi_defination.hpp"
namespace Helios
{
    class RHI
    {
    public:
        RHI() = default;
        //you must define Destructor here instead of pure function, otherwise you will get link error
        virtual ~RHI() {}; 
        virtual auto init(std::shared_ptr<Window>& window) -> void = 0;
        virtual auto create_platform_context() -> void = 0;
        virtual auto init_imgui_for_platform() -> void = 0;
        virtual auto create_buffer(RHI_Buffer_Create_info& buffer_create_info, RHI_Usage_Flag flag, uint32_t size, uint32_t stride) -> std::shared_ptr<RHI_Buffer> = 0;
        virtual auto create_vertex_array() -> std::shared_ptr<RHI_Vertex_Array> = 0;
        virtual auto create_shader(const std::string& path) -> std::shared_ptr<RHI_Shader> = 0;
        virtual auto create_texture(const std::string& path) -> std::shared_ptr<RHI_Texture> = 0;
        //virtual auto 
    private:
    };
    
}