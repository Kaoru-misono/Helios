#pragma once
#include <memory>
#include "window/window.hpp"
namespace Helios
{
    class RHI
    {
    public:
        virtual ~RHI() = 0; 
        virtual auto initialize(std::shared_ptr<Window>& window) -> void = 0;
    private:
    };
}