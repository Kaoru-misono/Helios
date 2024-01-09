#pragma once
namespace Helios
{
    template <typename T>
    auto inline lerp(T a, T b, T f) -> T
    {
        return a + f * (b - a);
    }
}
