#include "hatcher/assert.hpp"

namespace hatcher
{

template <typename T>
constexpr Vect<3, T>::Vect(T x, T y, T z)
    : x(x)
    , y(y)
    , z(z)
{
}

template <typename T>
constexpr Vect<3, T> Vect<3, T>::operator+(Self other) const
{
    return {x + other.x, y + other.y, z + other.z};
}

template <typename T>
constexpr Vect<3, T> Vect<3, T>::operator-(Self other) const
{
    return {x - other.x, y - other.y, z - other.z};
}

template <typename T>
constexpr Vect<3, T> Vect<3, T>::operator*(T scalar) const
{
    return {x * scalar, y * scalar, z * scalar};
}

template <typename T>
constexpr Vect<3, T> Vect<3, T>::operator/(T scalar) const
{
    return {x / scalar, y / scalar, z / scalar};
}

template <typename T>
constexpr Vect<3, T>& Vect<3, T>::operator+=(Self other)
{
    *this = *this + other;
    return *this;
}

template <typename T>
constexpr Vect<3, T>& Vect<3, T>::operator-=(Self other)
{
    *this = *this - other;
    return *this;
}

template <typename T>
constexpr Vect<3, T>& Vect<3, T>::operator*=(T scalar)
{
    *this = *this * scalar;
    return *this;
}

template <typename T>
constexpr Vect<3, T>& Vect<3, T>::operator/=(T scalar)
{
    *this = *this / scalar;
    return *this;
}

template <typename T>
constexpr bool Vect<3, T>::operator==(Self other) const
{
    return x == other.x && y == other.y && z == other.z;
}

template <typename T>
constexpr bool Vect<3, T>::operator!=(Self other) const
{
    return !operator==(other);
}

template <typename T>
constexpr T Vect<3, T>::operator[](int index) const
{
    return reinterpret_cast<const T*>(this)[index];
}

} // namespace hatcher
