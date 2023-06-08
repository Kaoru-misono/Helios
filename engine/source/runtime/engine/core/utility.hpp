#pragma once
#include "type.hpp"

namespace Helios::core
{
    namespace detail
    {
        template <class T> struct remove_reference      final { using type = T; };
        template <class T> struct remove_reference<T &> final { using type = T; };
        template <class T> struct remove_reference<T&&> final { using type = T; };

        template <class T> struct remove_const          final { using type = T; };
        template <class T> struct remove_const<T const> final { using type = T; };

        template <class T>           constexpr auto is_array = false;
        template <class T, size_t N> constexpr auto is_array<T [N]> = true;

        template <class T>           constexpr auto array_length = size_t(-1);
        template <class T, size_t N> constexpr auto array_length<T [N]> = N;

        template <class T>           struct array_element        final {};
        template <class T, size_t N> struct array_element<T [N]> final { using type = T; };

        template <class T, class U> constexpr auto is_same_type = false;
        template <class T>          constexpr auto is_same_type<T, T> = true;

        template <class T> constexpr auto is_const = false;
        template <class T> constexpr auto is_const<T const> = true;

        template <bool> struct conditional final { template <class T, class F> using type = F; };
        template <>     struct conditional<true> final { template <class T, class F> using type = T; };
    }

    template <class> constexpr auto dependent_true  = true;
    template <class> constexpr auto dependent_false = false;

    template <class T> struct type_tag final { using type = T; };
    template <auto V>  struct value_tag final { template <class T> static constexpr auto value = V; };

    template <class T> using type_identity = typename type_tag<T>::type;

    template <class T> using remove_reference = typename detail::remove_reference<T>::type;
    template <class T> using remove_const = typename detail::remove_const<T>::type;
    template <class T> using remove_cref = remove_const<remove_reference<T>>;

    template <class T> concept is_array = detail::is_array<remove_cref<T>>;
    template <class T> constexpr auto array_length = detail::array_length<remove_cref<T>>;
    template <class T> using array_element = typename detail::array_element<remove_cref<T>>::type;

    template <class T, class U> concept is_same_type = detail::is_same_type<T, U>;
    template <class T>          concept is_const = detail::is_const<T>;
    template <class T>          concept is_default_constructible = requires { T{}; };
    template <class T>          concept has_virtual_destructor = __has_virtual_destructor(T);
    template <class T>          concept is_complete_type = requires { sizeof(T); };

    template <class T, class U> concept is_implicit_convertible_to = requires (T x) { static_cast<void(*)(U)>(0)(x); };
    template <class T, class U> concept is_static_castable_to = requires (T x) { static_cast<U>(x); };
    template <class T, class U> concept is_castable_to = requires (T x) { (U)x; };
    template <class T, class U> concept is_base_of = __is_base_of(T, U);

    template <class T, class U> concept is_implicit_convertible_from = is_implicit_convertible_to<U, T>;
    template <class T, class U> concept is_static_castable_from = is_static_castable_to<U, T>;
    template <class T, class U> concept is_castable_from = is_castable_to<U, T>;
    template <class T, class U> concept is_based_on = is_base_of<U, T>;

    template <bool B, class T, class F> using conditional = typename detail::conditional<B>::template type<T, F>;

    template <class T, class U> using propagate_const = conditional<is_const<T>, U const, U>;

    template<class T>
    [[nodiscard]] constexpr auto declval() noexcept -> T&&
    {
        static_assert(dependent_false<T>, "The declval<T>() not allowed in an evaluated context.");
    }

    // Using "type_identity<T>" to prevent you from writing "forward(x)".
    // You must always write "forward<T>(x)".
    template <class T>
    [[nodiscard]] constexpr auto forward(type_identity<T>& x) noexcept -> decltype(auto)
    {
        return static_cast<T&&>(x);
    }

    template <class T>
    [[nodiscard]] constexpr auto move(T&& x) noexcept -> decltype(auto)
    {
        return static_cast<remove_reference<T>&&>(x);
    }

    template <class T, class U = T>
    constexpr auto exchange(T& a, U&& b) noexcept -> T
    {
        auto c = core::move(a);
        a = core::forward<U>(b);
        return c;
    }

    template <class T, class U = T>
    constexpr auto swap(T& a, U& b) noexcept -> void
    {
        auto c = core::move(a);
        a = core::move(b);
        b = core::move(c);
    }

    template <class Fn>
    struct scoped final
    {
        ~scoped() noexcept(noexcept(fn())) { fn(); }
        scoped(Fn&& fn) noexcept: fn{core::move(fn)} {}

    private:
        Fn fn;
    };

    template <class T> concept has_copy_constructor = requires (T& a) { T{a}; };
    template <class T> concept has_move_constructor = requires (T& a) { T{core::move(a)}; };
    template <class T> concept has_move_assign = requires (T& a, T& b) { a = core::move(b); };
    template <class T> concept has_copy_assign = requires (T& a, T& b) { a = b; };
    template <class T> concept can_move_construct = (has_move_constructor<T> || has_copy_constructor<T>);
    template <class T> concept can_move_assign = (can_move_construct<T> && (has_move_assign<T> || has_copy_assign<T>));

    [[nodiscard]] constexpr auto min(auto const& x0) noexcept -> auto const& { return x0; }
    [[nodiscard]] constexpr auto max(auto const& x0) noexcept -> auto const& { return x0; }

    template <class T>
    [[nodiscard]] constexpr auto min(T const& x0, T const& x1) noexcept -> T const& { return (x0 < x1 ? x0 : x1); }
    template <class T>
    [[nodiscard]] constexpr auto max(T const& x0, T const& x1) noexcept -> T const& { return (x0 < x1 ? x1 : x0); }

    [[nodiscard]] constexpr auto min(auto const& x0, auto const& x1, auto const&... xs) noexcept -> auto const& { return min(x0, min(x1, xs...)); }
    [[nodiscard]] constexpr auto max(auto const& x0, auto const& x1, auto const&... xs) noexcept -> auto const& { return max(x0, max(x1, xs...)); }

    [[nodiscard]] constexpr auto launder(auto* ptr) noexcept -> auto* { return __builtin_launder(ptr); }

    template <class Uint> [[nodiscard]] constexpr auto align_uint(Uint x, Uint alignment) noexcept -> Uint { return x + ((1 + ~x) & (alignment - 1)); }
    template <class T>    [[nodiscard]] constexpr auto align_ptr (T* x, size_t alignment) noexcept -> T*   { return reinterpret_cast<T*>(align_uint(reinterpret_cast<uintptr_t>(x), static_cast<uintptr_t>(alignment))); }

    template <class T, class U> requires (!is_const<U> || is_const<T>)
    [[nodiscard]] constexpr auto static_cast_ptr(U* ptr) noexcept -> T*
    {
        if constexpr (is_same_type<byte, remove_const<T>> || is_same_type<byte, remove_const<U>>) {
            return reinterpret_cast<T*>(ptr);
        } else {
            return static_cast<T*>(ptr);
        }
    }

    template <class T, class U> requires (sizeof(T) >= sizeof(U))
    [[nodiscard]] constexpr auto unsafe_union_cast(U const& x) noexcept -> decltype(auto)
    {
        union Box { U x; T y; };
        return Box{x}.y;
    }

    struct In_Place {};
    constexpr auto in_place = In_Place{};

    // Returns current time in seconds.
    // The epoch is unknown.
    // The clock is guaranteed to be monotonic.
    // The clock has highest resolution.
    // This is slow. Do not call it at high frequently.
    [[nodiscard]] auto now_seconds() noexcept -> float64_t;
}

[[nodiscard]] constexpr auto operator new   (::Helios::core::size_t, void* p, ::Helios::core::In_Place) noexcept -> void* { return p; }
[[nodiscard]] constexpr auto operator new[] (::Helios::core::size_t, void* p, ::Helios::core::In_Place) noexcept -> void* { return p; }
constexpr auto operator delete   (void*, void*, ::Helios::core::In_Place) noexcept -> void {}
constexpr auto operator delete[] (void*, void*, ::Helios::core::In_Place) noexcept -> void {}

