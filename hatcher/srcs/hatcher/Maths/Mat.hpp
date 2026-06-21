#pragma once

#include <cstddef>

#include "Vect.hpp"

namespace hatcher
{

template <std::size_t L, typename T>
struct Mat;

template <std::size_t L, std::size_t VL, typename T>
constexpr Vect<VL, T> operator*(const Mat<L, T>& u, const Vect<VL, T>& v);

template <std::size_t L, typename T>
constexpr Mat<L, T> operator*(const Mat<L, T>& u, const Mat<L, T>& v);

template <std::size_t L, typename T>
constexpr Mat<L, T>& operator*=(Mat<L, T>& u, const Mat<L, T>& v);

} // namespace hatcher

#include "Mat/Mat3.hpp"
#include "Mat/Mat4.hpp"

#include "Mat.ipp"