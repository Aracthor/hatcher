#include <cmath>

namespace hatcher
{

template <typename T>
constexpr T DegToRad(T angle)
{
    static_assert(std::numeric_limits<T>::is_iec559, "'DegToRad' is only possible for floating-point type.");
    return M_PI / T(180) * angle;
}

template <typename T>
constexpr T RadToDeg(T angle)
{
    static_assert(std::numeric_limits<T>::is_iec559, "'RadToDeg' is only possible for floating-point type.");
    return T(180) / M_PI * angle;
}

template <typename T>
T OrientedAngle(Vect<2, T> u, Vect<2, T> v)
{
    static_assert(std::numeric_limits<T>::is_iec559, "'OrientedAngle' is only possible for floating-point type.");
    const T angle = std::acos(Dot(u, v));
    const T det = u.x * v.y - u.y * v.x;
    if (det >= 0.f)
        return angle;
    return -angle;
}

} // namespace hatcher
