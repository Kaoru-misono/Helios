#pragma once

namespace Helios::core
{
    inline namespace distinct_primitive
    {
        using float32_t = float;
        using float64_t = double;

        using int8_t = signed char;
        using int16_t = signed short;
        using int32_t = signed int;

        using uint8_t = unsigned char;
        using uint16_t = unsigned short;
        using uint32_t = unsigned int;


        using int64_t = signed long long;
        using uint64_t = unsigned long long;
    }

    inline namespace aliased_primitive
    {
        using size_t    = decltype(sizeof(0));
        using ptrdiff_t = decltype((char*)0 - (char*)0);

        using  intptr_t = ptrdiff_t;
        using  intmax_t = int64_t;
        using uintptr_t = size_t;
        using uintmax_t = uint64_t;

        struct alignas(16) max_align_t final {};

        using nullptr_t = decltype(nullptr);

        enum struct byte: uint8_t {};
    }
}
