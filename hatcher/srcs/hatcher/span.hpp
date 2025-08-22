#pragma once

#include <vector>

namespace hatcher
{

template <typename T>
class span
{
public:
    constexpr span() = default;

    constexpr span(T* pointer, std::size_t size)
        : ptr(pointer)
        , length(size)
    {
    }

    template <typename U>
    constexpr span(std::vector<U>& container)
        : ptr(container.data())
        , length(container.size())
    {
    }
    template <typename U>
    constexpr span(const std::vector<U>& container)
        : ptr(container.data())
        , length(container.size())
    {
    }

    std::size_t size() const { return length; }

    T* begin() { return ptr; }
    T* end() { return ptr + length; }

    const T* begin() const { return ptr; }
    const T* end() const { return ptr + length; }

    T& operator[](int idx) { return ptr[idx]; }
    const T& operator[](int idx) const { return ptr[idx]; }

private:
    T* ptr;
    std::size_t length;
};
} // namespace hatcher
