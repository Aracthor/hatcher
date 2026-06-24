#pragma once

#include <cstddef>

namespace hatcher
{

template <std::size_t L, typename T>
struct Vec;

template <std::size_t L, typename T>
constexpr T Dot(Vec<L, T> u, Vec<L, T> v);

} // namespace hatcher

#include "Vec/Vec1.hpp"
#include "Vec/Vec2.hpp"
#include "Vec/Vec3.hpp"
#include "Vec/Vec4.hpp"

#include "Vec.ipp"
