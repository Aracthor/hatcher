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
    explicit constexpr Vect(T scalar);
    constexpr Vect(T x, T y);

    constexpr Self& operator=(const Self& other) = default;
    constexpr Self& operator=(Self&& other) = default;

    constexpr Self operator-() const;

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

    template <typename T2>
    constexpr operator Vect<2, T2>() const;

    constexpr T Length() const;
    constexpr Self Normalized() const;
};

using Vect2f = Vect<2, float>;
using Vect2i = Vect<2, int>;

} // namespace hatcher

#include "Vect2.ipp"
