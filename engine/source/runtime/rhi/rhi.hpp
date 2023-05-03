#pragma once
#include <memory>
#include "window/window.hpp"
namespace Helios
{
    class RHI
    {
    public:
        RHI() = default;
        virtual ~RHI() = 0; 
        virtual auto initialize(std::shared_ptr<Window>& window) -> void = 0;
        virtual auto create_context() -> void = 0;
    private:
    };
    //you must define Destructor here otherwise link error
    inline RHI::~RHI() = default;
}