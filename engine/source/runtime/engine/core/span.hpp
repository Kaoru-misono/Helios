#pragma once
// A `Span<T>` is a window into an array, with which you can read/write content of a part of the array.
// A `Span<T const>` is a read-only window into an array, with which you can read content of a part of the array.
//
// Spans do NOT own the array. Spans borrow the array.
// Spans are cheap and you should pass spans by value.
//
// Spans are immutable. Functions that modify the span will actually
// create a new copy of the span and apply the modification.
//
// **Char arrays are special cased.**
//
// Constructing a span with a char array ignores the sentinel NUL '\0' character,
// so that `Span{"string literal"}` will have the correct `.size() == size_t(14)`.
// Unfortunately, this makes a span of `char buf[1024]` have 1 less element
// `Span{buf}.size() == size_t(1023)`. You should use `byte buffer[1024]` instead.
#include "type.hpp"
#include "utility.hpp"

namespace Helios::core
{
    template <class T>
    struct Span;

    template <class T>
        requires (requires (T& a) { a.begin(); })
    Span(T&) -> Span<core::remove_reference<decltype(*(core::declval<T>().begin()))>>;

    template <class T, core::size_t N>
    Span(T (&)[N]) -> Span<T>;

    template <class Iter>
    Span(Iter first, Iter last) -> Span<core::remove_reference<decltype(*Iter{})>>;

    template <class Iter>
    Span(Iter first, core::size_t size) -> Span<core::remove_reference<decltype(*Iter{})>>;

    namespace span_detail
    {
        template <class T>
        constexpr auto begin(T&& x) noexcept -> auto*
        {
            if constexpr (core::is_array<T>) {
                return &x[0];
            } else {
                return &*x.begin();
            }
        }

        template <class T>
        constexpr auto end(T&& x) noexcept -> auto*
        {
            if constexpr (core::is_array<T>) {
                if constexpr (core::is_same_type<core::array_element<T>, char>) {
                    if constexpr (core::array_length<T> >= 1) {
                        return &x[core::array_length<T> - 1];
                    } else {
                        return &x[0];
                    }
                } else {
                    return &x[core::array_length<T>];
                }
            } else {
                return &*x.end();
            }
        }
    }

    template <class T>
    struct Span final
    {
        using size_type = size_t;
        using index_type = ptrdiff_t;

        using value_type = T;
        using const_value_type = value_type const;
        using mutable_value_type = core::remove_const<value_type>;

        using pointer = value_type*;
        using const_pointer = const_value_type*;
        using mutable_pointer = mutable_value_type*;

        using reference = value_type&;
        using const_reference = const_value_type&;
        using mutable_reference = mutable_value_type&;

        using iterator = pointer;
        using const_iterator = const_pointer;
        using mutable_iterator = mutable_pointer;

        ~Span() = default;
        constexpr Span() = default;

        template <class Iter>
        constexpr Span(Iter first, Iter last) noexcept
            // Use "&*iter" instead of "iter" to make sure that iterators are converted to pointers.
            : first{&*first}, last{&*last}
        {}

        template <class Iter>
        constexpr Span(Iter first, size_type size) noexcept
            : Span{first, first + size}
        {}

        // A single universal reference to handle all the following cases:
        // - Copying of Span
        // - Moving  of Span
        // - Span<T> to Span<T const>
        // - Container<T> to Span<T>
        // - T[N] to Span<T>
        template <class U>
        constexpr Span(U&& x) noexcept
            : Span{span_detail::begin(core::forward<U>(x)), span_detail::end(core::forward<U>(x))}
        {}

        template <class U>
        constexpr auto operator = (U&& x) noexcept -> Span&
        {
            first = span_detail::begin(core::forward<U>(x));
             last = span_detail::end  (core::forward<U>(x));
            return *this;
        }

        constexpr auto empty() const noexcept -> bool { return (first >= last); }

        constexpr auto begin() const noexcept -> iterator { return first; }
        constexpr auto   end() const noexcept -> iterator { return core::max(first, last); }

        constexpr auto cbegin() const noexcept -> const_iterator { return first; }
        constexpr auto   cend() const noexcept -> const_iterator { return core::max(first, last); }

        constexpr auto front() const noexcept -> reference { return first[0]; }
        constexpr auto  back() const noexcept -> reference { return last[-1]; }

        constexpr auto data() const noexcept -> pointer { return first; }
        constexpr auto size() const noexcept -> size_type { return (empty() ? size_type{} : size_type(last - first)); }

        constexpr auto operator [] (index_type idx) const noexcept -> reference { return first[idx]; }

        template <class U> requires (requires { U{pointer{}, pointer{}}; })
        explicit constexpr operator U () const noexcept { return U{first, last}; }

        template <class U> requires (requires { U{pointer{}, pointer{}}; })
        constexpr auto clone_into() const noexcept -> U { return U{first, last}; }

        // - Span<T>            can cast to Span<byte>  or Span<byte const>.
        // - Span<byte>         can cast to Span<T>     or Span<T const>.
        // - Span<T const>      can cast to Span<byte const>.
        // - Span<byte const>   can cast to Span<T const>.
        // - Otherwise, casting Span<T> to Span<U> is possible if static casting T* to U* is possible.
        template <class U>
        constexpr auto cast() const noexcept -> Span<U>
        {
            return {core::static_cast_ptr<U>(first), core::static_cast_ptr<U>(last)};
        }

        constexpr auto read_only() const noexcept -> Span<const_value_type> { return cast<const_value_type>(); }

        constexpr auto as_bytes() const noexcept -> auto { return cast<core::propagate_const<value_type, byte>>(); }

        constexpr auto align_first(size_t alignment) const noexcept -> Span { return reset_first(core::align_ptr(first, alignment)); }
        constexpr auto align_last (size_t alignment) const noexcept -> Span { return reset_last (core::align_ptr(last,  alignment)); }

        // When `op(x).first < op(x).last`, it is undefined behavior if
        // - `op(x).first < this->first`, or
        // - `op(x).last  > this->last`.
        // where op(x) is any of the following 6 functions.
        constexpr auto reset_first    (pointer x) const noexcept -> Span { return {        x,  last    }; }
        constexpr auto reset_last     (pointer x) const noexcept -> Span { return {first,             x}; }
        constexpr auto remove_prefix(size_type x) const noexcept -> Span { return {first + x,  last    }; }
        constexpr auto remove_suffix(size_type x) const noexcept -> Span { return {first,      last - x}; }
        constexpr auto prefix       (size_type x) const noexcept -> Span { return {first,     first + x}; }
        constexpr auto suffix       (size_type x) const noexcept -> Span { return { last - x,  last    }; }

        // Find the first element that is one of the xs.
        // Returns end() if not found.
        template <class... Ts>
        constexpr auto find_first_of(Ts&&... xs) const noexcept -> iterator
        {
            for (auto& v: *this)
                if ((... || (v == xs)))
                    return &v;
            return end();
        }

        // Find the first element that is not any of the xs.
        // Returns end() if not found.
        template <class... Ts>
        constexpr auto find_first_not_of(Ts&&... xs) const noexcept -> iterator
        {
            for (auto& v: *this)
                if (!(... || (v == xs)))
                    return &v;
            return end();
        }

        // Find the last element that is one of the xs, and returns the element AFTER it.
        // Returns begin() if not found.
        template <class... Ts>
        constexpr auto find_last_of(Ts&&... xs) const noexcept -> iterator
        {
            for (auto p=end(); p>begin(); p--)
                if ((... || (p[-1] == xs)))
                    return p;
            return begin();
        }

        // Find the last element that is not any of the xs, and returns the element AFTER it.
        // Returns begin() if not found.
        template <class... Ts>
        constexpr auto find_last_not_of(Ts&&... xs) const noexcept -> iterator
        {
            for (auto p=end(); p>begin(); p--)
                if (!(... || (p[-1] == xs)))
                    return p;
            return begin();
        }

        constexpr auto starts_with(Span<const_value_type> x) const noexcept -> bool { return (size() >= x.size() && read_only().prefix(x.size()) == x); }
        constexpr auto   ends_with(Span<const_value_type> x) const noexcept -> bool { return (size() >= x.size() && read_only().suffix(x.size()) == x); }

        constexpr auto unsafe_mutate() const noexcept -> Span<mutable_value_type> { return {(mutable_pointer) first, (mutable_pointer) last}; }

    private:
        pointer first{};
        pointer last{};
    };

    template <class T, class U>
    constexpr auto operator == (Span<T> a, Span<U> b) noexcept -> bool
    {
        if (a.size() != b.size())
            return false;

        auto p = b.begin();
        for (auto& x: a)
            if (!(x == *p++))
                return false;

        return true;
    }

    template <class T, class U>
    constexpr auto operator != (Span<T> a, Span<U> b) noexcept -> bool
    {
        return !(a == b);
    }

    template <class T, class U> constexpr auto operator == (Span<T> a, U&& b) noexcept -> bool { return (a == Span{b}); }
    template <class T, class U> constexpr auto operator == (U&& a, Span<T> b) noexcept -> bool { return (Span{a} == b); }
    template <class T, class U> constexpr auto operator != (Span<T> a, U&& b) noexcept -> bool { return (a != Span{b}); }
    template <class T, class U> constexpr auto operator != (U&& a, Span<T> b) noexcept -> bool { return (Span{a} != b); }

    template <class T, class U>
    constexpr auto is_same(Span<T> a, Span<U> b) noexcept -> bool
    {
        if constexpr (is_same_type<remove_cref<T>, remove_cref<U>>) {
            if (a.empty() && b.empty()) return true;
            if (a.begin() != b.begin()) return false;
            if (a.end() != b.end()) return false;
            return true;
        } else {
            return false;
        }
    }
}
