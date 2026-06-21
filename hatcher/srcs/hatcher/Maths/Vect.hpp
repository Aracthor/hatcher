#pragma once

namespace hatcher
{

template <std::size_t L, typename T>
struct Vect;

template <std::size_t L, typename T>
constexpr T dot(Vect<L, T> u, Vect<L, T> v);

} // namespace hatcher

#include "Vect/Vect2.hpp"
#include "Vect/Vect3.hpp"
#include "Vect/Vect4.hpp"

#include "Vect.ipp"
