#pragma once

namespace hatcher
{
template <typename T>
struct Vec<2, T>
{
    using Self = Vec<2, T>;

    T x;
    T y;

    constexpr Vec() = default;
    constexpr Vec(const Self& other) = default;
    constexpr Vec(Self&& other) = default;
    explicit constexpr Vec(T scalar);
    constexpr Vec(T x, T y);

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
    constexpr operator Vec<2, T2>() const;

    constexpr T Length() const;
    constexpr T LengthSqr() const;
    constexpr Self Normalized() const;
};

using Vec2f = Vec<2, float>;
using Vec2i = Vec<2, int>;

} // namespace hatcher

#include "Vec2.ipp"
