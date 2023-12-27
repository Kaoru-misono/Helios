#pragma once
#include "../../pre-compile.h"

namespace Helios
{
    template <class Instance>
    struct Singleton
    {
        static auto instance() -> Instance&;
        ~Singleton() { instance_ = nullptr; }
        Singleton(Singleton const&) = delete;
        Singleton(Singleton     &&) = delete;
        Singleton & operator = (Singleton const&) = delete;
        Singleton & operator = (Singleton     &&) = delete;
    protected:
        Singleton() = default;
    private:
        static std::unique_ptr<Instance> instance_;
    };

    template <class Instance>
    inline auto Singleton<Instance>::instance() -> Instance&
    {
        // Single thread safe
        if (instance_ == nullptr) {
            instance_ = std::make_unique<Instance>();
        }
        return *instance_;
    }

    template <class Instance>
    std::unique_ptr<Instance> Singleton<Instance>::instance_ = nullptr;
}
