#pragma once

namespace hatcher
{
template <typename T>
struct Vec<3, T>
{
    using Self = Vec<3, T>;

    T x;
    T y;
    T z;

    constexpr Vec() = default;
    constexpr Vec(const Self& other) = default;
    constexpr Vec(Self&& other) = default;
    explicit constexpr Vec(T scalar);
    constexpr Vec(T x, T y, T z);
    constexpr Vec(Vec<2, T> v, T z);

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
    constexpr operator Vec<3, T2>() const;

    constexpr const T* Data() const;
    constexpr Vec<2, T> xy() const;

    constexpr T Length() const;
    constexpr T LengthSqr() const;
    constexpr Self Normalized() const;
};

template <typename T>
constexpr Vec<3, T> Cross(Vec<3, T> u, Vec<3, T> v);

using Vec3f = Vec<3, float>;

} // namespace hatcher

#include "Vec3.ipp"
