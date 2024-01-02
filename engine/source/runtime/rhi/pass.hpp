#pragma once
#include "rhi/rhi_defination.hpp"
#include "rhi/texture.hpp"
#include "draw_command.hpp"
#include "render_state.hpp"
#include "../pre-compile.h"

namespace Helios
{
    struct Pass
    {
        enum struct Depth_Function
        {
            less,
            equal,
            less_equal,
        };
        virtual ~Pass() {}
        virtual auto shader_process() -> void = 0;
        virtual auto update() -> void = 0;
        virtual auto render() -> void = 0;
        virtual auto set_uniform(std::string name, std::any uniform) -> void = 0;
        virtual auto set_sampler(std::string name, Texture_Sampler sampler) -> void = 0;
        bool enable_depth_test{true};
        bool enable_depth_write{false};
        Depth_Function depth_func;
        Clear_State clear_state{};
        std::shared_ptr<RHI_Framebuffer> frame_buffer{};
        std::vector<RHI_Draw_Command> queue;
        std::string vertex_shader{};
        std::string fragment_shader{};
        std::string geometry_shader{};
        std::string name{};
    protected:
        std::unique_ptr<RHI_GPU_Program> gpu_program{};
        //will be set once and cross every frame
        std::unordered_map<std::string, std::any> uniforms{};
        std::unordered_map<std::string, Texture_Sampler> samplers{};
    };
}
