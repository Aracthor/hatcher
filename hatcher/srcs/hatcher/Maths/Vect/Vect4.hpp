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
    explicit constexpr Vect(T scalar);
    constexpr Vect(T x, T y, T z, T w);
    constexpr Vect(Vect<3, T> v, T w);

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
    constexpr operator Vect<4, T2>() const;

    constexpr const T* Data() const;
    constexpr Vect<3, T> xyz() const;

    constexpr T Length() const;
    constexpr Self Normalized() const;
};

using Vect4f = Vect<4, float>;

} // namespace hatcher

#include "Vect4.ipp"
