#include "hatcher/assert.hpp"

namespace hatcher
{

template <typename T>
constexpr Vect<4, T>::Vect(T x, T y, T z, T w)
    : x(x)
    , y(y)
    , z(z)
    , w(w)
{
}

template <typename T>
constexpr Vect<4, T> Vect<4, T>::operator+(Self other) const
{
    return {x + other.x, y + other.y, z + other.z, w + other.w};
}

template <typename T>
constexpr Vect<4, T> Vect<4, T>::operator-(Self other) const
{
    return {x - other.x, y - other.y, z - other.z, w - other.w};
}

template <typename T>
constexpr Vect<4, T> Vect<4, T>::operator*(T scalar) const
{
    return {x * scalar, y * scalar, z * scalar, w * scalar};
}

template <typename T>
constexpr Vect<4, T> Vect<4, T>::operator/(T scalar) const
{
    return {x / scalar, y / scalar, z / scalar, w / scalar};
}

template <typename T>
constexpr Vect<4, T>& Vect<4, T>::operator+=(Self other)
{
    *this = *this + other;
    return *this;
}

template <typename T>
constexpr Vect<4, T>& Vect<4, T>::operator-=(Self other)
{
    *this = *this - other;
    return *this;
}

template <typename T>
constexpr Vect<4, T>& Vect<4, T>::operator*=(T scalar)
{
    *this = *this * scalar;
    return *this;
}

template <typename T>
constexpr Vect<4, T>& Vect<4, T>::operator/=(T scalar)
{
    *this = *this / scalar;
    return *this;
}

template <typename T>
constexpr bool Vect<4, T>::operator==(Self other) const
{
    return x == other.x && y == other.y && z == other.z && w == other.w;
}

template <typename T>
constexpr bool Vect<4, T>::operator!=(Self other) const
{
    return !operator==(other);
}

template <typename T>
constexpr T Vect<4, T>::operator[](int index) const
{
    return reinterpret_cast<const T*>(this)[index];
}

template <typename T>
constexpr T Vect<4, T>::length() const
{
    static_assert(std::numeric_limits<T>::is_iec559, "'length' is only possible for floating-point Vect.");
    return std::sqrt(x * x + y * y + z * z + w * w);
}

} // namespace hatcher
