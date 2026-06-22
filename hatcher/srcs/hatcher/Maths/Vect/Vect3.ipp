#include "hatcher/assert.hpp"

namespace hatcher
{

template <typename T>
constexpr Vect<3, T>::Vect(T scalar)
    : x(scalar)
    , y(scalar)
    , z(scalar)
{
}

template <typename T>
constexpr Vect<3, T>::Vect(T x, T y, T z)
    : x(x)
    , y(y)
    , z(z)
{
}

template <typename T>
constexpr Vect<3, T>::Vect(Vect<2, T> v, T z)
    : x(v.x)
    , y(v.y)
    , z(z)
{
}

template <typename T>
constexpr Vect<3, T> Vect<3, T>::operator-() const
{
    return *this * -1;
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
template <typename T2>
constexpr Vect<3, T>::operator Vect<3, T2>() const
{
    return Vect<3, T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z));
}

template <typename T>
constexpr const T* Vect<3, T>::Data() const
{
    static_assert(sizeof(*this) == sizeof(T) * 3);
    return reinterpret_cast<const T*>(this);
}

template <typename T>
constexpr Vect<2, T> Vect<3, T>::xy() const
{
    return Vect<2, T>(x, y);
}

template <typename T>
constexpr T Vect<3, T>::Length() const
{
    static_assert(std::numeric_limits<T>::is_iec559, "'length' is only possible for floating-point Vect.");
    return std::sqrt(LengthSqr());
}

template <typename T>
constexpr T Vect<3, T>::LengthSqr() const
{
    return x * x + y * y + z * z;
}

template <typename T>
constexpr Vect<3, T> Vect<3, T>::Normalized() const
{
    static_assert(std::numeric_limits<T>::is_iec559, "'length' is only possible for floating-point Vect.");
    return *this / Length();
}

template <typename T>
constexpr Vect<3, T> Cross(Vect<3, T> u, Vect<3, T> v)
{
    return {
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x,
    };
}

} // namespace hatcher
