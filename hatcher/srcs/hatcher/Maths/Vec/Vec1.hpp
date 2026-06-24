#pragma once

namespace hatcher
{
template <typename T>
struct Vec<1, T>
{
    using Self = Vec<1, T>;

    T x;

    constexpr Vec() = default;
    constexpr Vec(const Self& other) = default;
    constexpr Vec(Self&& other) = default;
    constexpr Vec(T scalar);

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

    constexpr operator T() const;

    template <typename T2>
    constexpr operator Vec<1, T2>() const;

    constexpr T Length() const;
    constexpr T LengthSqr() const;
    constexpr Self Normalized() const;
};

using Vec1f = Vec<1, float>;

} // namespace hatcher

#include "Vec1.ipp"
