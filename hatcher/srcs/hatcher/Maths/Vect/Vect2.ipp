#include "hatcher/assert.hpp"

namespace hatcher
{

template <typename T>
constexpr Vect<2, T>::Vect(T x, T y)
    : x(x)
    , y(y)
{
}

template <typename T>
constexpr Vect<2, T> Vect<2, T>::operator+(Self other) const
{
    return {x + other.x, y + other.y};
}

template <typename T>
constexpr Vect<2, T> Vect<2, T>::operator-(Self other) const
{
    return {x - other.x, y - other.y};
}

template <typename T>
constexpr Vect<2, T> Vect<2, T>::operator*(T scalar) const
{
    return {x * scalar, y * scalar};
}

template <typename T>
constexpr Vect<2, T> Vect<2, T>::operator/(T scalar) const
{
    return {x / scalar, y / scalar};
}

template <typename T>
constexpr Vect<2, T>& Vect<2, T>::operator+=(Self other)
{
    *this = *this + other;
    return *this;
}

template <typename T>
constexpr Vect<2, T>& Vect<2, T>::operator-=(Self other)
{
    *this = *this - other;
    return *this;
}

template <typename T>
constexpr Vect<2, T>& Vect<2, T>::operator*=(T scalar)
{
    *this = *this * scalar;
    return *this;
}

template <typename T>
constexpr Vect<2, T>& Vect<2, T>::operator/=(T scalar)
{
    *this = *this / scalar;
    return *this;
}

template <typename T>
constexpr bool Vect<2, T>::operator==(Self other) const
{
    return x == other.x && y == other.y;
}

template <typename T>
constexpr bool Vect<2, T>::operator!=(Self other) const
{
    return !operator==(other);
}

template <typename T>
constexpr T Vect<2, T>::operator[](int index) const
{
    return reinterpret_cast<const T*>(this)[index];
}

} // namespace hatcher
