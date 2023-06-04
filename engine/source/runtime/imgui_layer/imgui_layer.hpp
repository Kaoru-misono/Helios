#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Helios
{
    class ImGui_Layer final
    {
    public:
        auto init() -> void;
        auto destroy() -> void;

        auto update() -> void;
        auto render() -> void;

    private:
    };
}