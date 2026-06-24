#pragma once

namespace hatcher
{
template <typename T>
struct Vec<4, T>
{
    using Self = Vec<4, T>;

    T x;
    T y;
    T z;
    T w;

    constexpr Vec() = default;
    constexpr Vec(const Self& other) = default;
    constexpr Vec(Self&& other) = default;
    explicit constexpr Vec(T scalar);
    constexpr Vec(T x, T y, T z, T w);
    constexpr Vec(Vec<3, T> v, T w);

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
    constexpr operator Vec<4, T2>() const;

    constexpr const T* Data() const;
    constexpr Vec<3, T> xyz() const;

    constexpr T Length() const;
    constexpr T LengthSqr() const;
    constexpr Self Normalized() const;
};

using Vec4f = Vec<4, float>;

} // namespace hatcher

#include "Vec4.ipp"
