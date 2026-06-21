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

template <typename T>
constexpr T& Vect<3, T>::operator[](int index)
{
    return reinterpret_cast<T*>(this)[index];
}

template <typename T>
constexpr T Vect<3, T>::length() const
{
    static_assert(std::numeric_limits<T>::is_iec559, "'length' is only possible for floating-point Vect.");
    return std::sqrt(x * x + y * y + z * z);
}

template <typename T>
constexpr Vect<3, T> Vect<3, T>::normalized() const
{
    static_assert(std::numeric_limits<T>::is_iec559, "'length' is only possible for floating-point Vect.");
    return *this / length();
}

template <typename T>
constexpr Vect<3, T> cross(Vect<3, T> u, Vect<3, T> v)
{
    return {
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x,
    };
}

} // namespace hatcher
