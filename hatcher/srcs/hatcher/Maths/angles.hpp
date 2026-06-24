#pragma once

#include "Vec.hpp"

namespace hatcher
{

template <typename T>
constexpr T DegToRad(T angle);
template <typename T>
constexpr T RadToDeg(T angle);

template <typename T>
T Angle(Vec<2, T> vNormalized);

template <typename T>
T OrientedAngle(Vec<2, T> u, Vec<2, T> v);

} // namespace hatcher

#include "angles.ipp"
