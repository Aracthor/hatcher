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
    explicit constexpr Vect(T scalar);
    constexpr Vect(T x, T y, T z);
    constexpr Vect(Vect<2, T> v, T z);

    constexpr Self& operator=(const Self& other) = default;
    constexpr Self& operator=(Self&& other) = default;

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
    constexpr operator Vect<3, T2>() const;

    constexpr T Length() const;
    constexpr Self Normalized() const;
};

template <typename T>
constexpr Vect<3, T> Cross(Vect<3, T> u, Vect<3, T> v);

using Vect3f = Vect<3, float>;

} // namespace hatcher

#include "Vect3.ipp"
