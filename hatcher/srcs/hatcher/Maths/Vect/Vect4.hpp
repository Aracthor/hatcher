#pragma once

namespace hatcher
{
template <typename T>
struct Vect<4, T>
{
    using Self = Vect<4, T>;

    T x;
    T y;
    T z;
    T w;

    constexpr Vect() = default;
    constexpr Vect(const Self& other) = default;
    constexpr Vect(Self&& other) = default;
    constexpr Vect(T x, T y, T z, T w);
    constexpr Vect(Vect<3, T> v, T w);

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

using Vect4f = Vect<4, float>;

} // namespace hatcher

#include "Vect4.ipp"
