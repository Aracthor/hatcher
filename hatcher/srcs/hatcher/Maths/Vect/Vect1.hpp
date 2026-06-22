#pragma once

namespace hatcher
{
template <typename T>
struct Vect<1, T>
{
    using Self = Vect<1, T>;

    T x;

    constexpr Vect() = default;
    constexpr Vect(const Self& other) = default;
    constexpr Vect(Self&& other) = default;
    constexpr Vect(T scalar);

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
    constexpr operator Vect<1, T2>() const;

    constexpr T Length() const;
    constexpr Self Normalized() const;
};

using Vect1f = Vect<1, float>;

} // namespace hatcher

#include "Vect1.ipp"
