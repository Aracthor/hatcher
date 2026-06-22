#pragma once

#include "Vect.hpp"

namespace hatcher
{

template <typename T>
constexpr T DegToRad(T angle);
template <typename T>
constexpr T RadToDeg(T angle);

template <typename T>
T OrientedAngle(Vect<2, T> u, Vect<2, T> v);

} // namespace hatcher

#include "angles.ipp"
