#pragma once

namespace hatcher
{
template <typename T>
struct Vect<2, T>
{
    using Self = Vect<2, T>;

    T x;
    T y;

    constexpr Vect() = default;
    constexpr Vect(const Self& other) = default;
    constexpr Vect(Self&& other) = default;
    constexpr Vect(T x, T y);

    constexpr Self operator+(Self other) const;
    constexpr Self operator-(Self other) const;
    constexpr Self operator*(T scalar) const;
    constexpr Self operator/(T scalar) const;

    constexpr Self& operator+=(Self other);
    constexpr Self& operator-=(Self other);
    constexpr Self& operator*=(T scalar);
    constexpr Self& operator/=(T scalar);

    constexpr bool operator==(Self other) const;
    constexpr bool operator!=(Self other) const;

    constexpr T operator[](int index) const;
    constexpr T& operator[](int index);

    constexpr T length() const;
    constexpr Self normalized() const;
};

using Vect2f = Vect<2, float>;

} // namespace hatcher

#include "Vect2.ipp"
