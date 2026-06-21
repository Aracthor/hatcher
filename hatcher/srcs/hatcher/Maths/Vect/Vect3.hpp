#pragma once

namespace hatcher
{
template <typename T>
struct Vect<3, T>
{
    using Self = Vect<3, T>;

    T x;
    T y;
    T z;

    constexpr Vect() = default;
    constexpr Vect(const Self& other) = default;
    constexpr Vect(Self&& other) = default;
    constexpr Vect(T x, T y, T z);

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
};

template <typename T>
constexpr Vect<3, T> cross(Vect<3, T> u, Vect<3, T> v);

using Vect3f = Vect<3, float>;

} // namespace hatcher

#include "Vect3.ipp"
