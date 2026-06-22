#include <cmath>
#include <limits>

#include "hatcher/assert.hpp"

namespace hatcher
{

template <typename T>
constexpr Vect<1, T>::Vect(T scalar)
    : x(scalar)
{
}

template <typename T>
constexpr Vect<1, T> Vect<1, T>::operator+(Self other) const
{
    return {x + other.x};
}

template <typename T>
constexpr Vect<1, T> Vect<1, T>::operator-(Self other) const
{
    return {x - other.x};
}

template <typename T>
constexpr Vect<1, T> Vect<1, T>::operator*(T scalar) const
{
    return {x * scalar};
}

template <typename T>
constexpr Vect<1, T> Vect<1, T>::operator/(T scalar) const
{
    return {x / scalar};
}

template <typename T>
constexpr Vect<1, T>& Vect<1, T>::operator+=(Self other)
{
    *this = *this + other;
    return *this;
}

template <typename T>
constexpr Vect<1, T>& Vect<1, T>::operator-=(Self other)
{
    *this = *this - other;
    return *this;
}

template <typename T>
constexpr Vect<1, T>& Vect<1, T>::operator*=(T scalar)
{
    *this = *this * scalar;
    return *this;
}

template <typename T>
constexpr Vect<1, T>& Vect<1, T>::operator/=(T scalar)
{
    *this = *this / scalar;
    return *this;
}

template <typename T>
constexpr bool Vect<1, T>::operator==(Self other) const
{
    return x == other.x;
}

template <typename T>
constexpr bool Vect<1, T>::operator!=(Self other) const
{
    return !operator==(other);
}

template <typename T>
constexpr T Vect<1, T>::operator[](int index) const
{
    return reinterpret_cast<const T*>(this)[index];
}

template <typename T>
constexpr T& Vect<1, T>::operator[](int index)
{
    return reinterpret_cast<T*>(this)[index];
}

template <typename T>
constexpr Vect<1, T>::operator T() const
{
    return x;
}

template <typename T>
template <typename T2>
constexpr Vect<1, T>::operator Vect<1, T2>() const
{
    return Vect<1, T2>(static_cast<T2>(x));
}

template <typename T>
constexpr T Vect<1, T>::Length() const
{
    static_assert(std::numeric_limits<T>::is_iec559, "'Length' is only possible for floating-point Vect.");
    return x;
}

template <typename T>
constexpr Vect<1, T> Vect<1, T>::Normalized() const
{
    static_assert(std::numeric_limits<T>::is_iec559, "'Normalized' is only possible for floating-point Vect.");
    return T(1);
}

} // namespace hatcher
