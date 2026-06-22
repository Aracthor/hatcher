#include "hatcher/assert.hpp"

namespace hatcher
{

template <typename T>
constexpr Vect<4, T>::Vect(T scalar)
    : x(scalar)
    , y(scalar)
    , z(scalar)
    , w(scalar)
{
}

template <typename T>
constexpr Vect<4, T>::Vect(T x, T y, T z, T w)
    : x(x)
    , y(y)
    , z(z)
    , w(w)
{
}

template <typename T>
constexpr Vect<4, T>::Vect(Vect<3, T> v, T w)
    : x(v.x)
    , y(v.y)
    , z(v.z)
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
    return Data()[index];
}

template <typename T>
constexpr T& Vect<4, T>::operator[](int index)
{
    return reinterpret_cast<T*>(this)[index];
}

template <typename T>
template <typename T2>
constexpr Vect<4, T>::operator Vect<4, T2>() const
{
    return Vect<4, T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z), static_cast<T2>(w));
}

template <typename T>
constexpr const T* Vect<4, T>::Data() const
{
    static_assert(sizeof(*this) == sizeof(T) * 4);
    return reinterpret_cast<const T*>(this);
}

template <typename T>
constexpr Vect<3, T> Vect<4, T>::xyz() const
{
    return Vect<3, T>(x, y, z);
}

template <typename T>
constexpr T Vect<4, T>::Length() const
{
    static_assert(std::numeric_limits<T>::is_iec559, "'Length' is only possible for floating-point Vect.");
    return std::sqrt(x * x + y * y + z * z + w * w);
}

template <typename T>
constexpr Vect<4, T> Vect<4, T>::Normalized() const
{
    static_assert(std::numeric_limits<T>::is_iec559, "'Normalized' is only possible for floating-point Vect.");
    return *this / Length();
}

} // namespace hatcher
