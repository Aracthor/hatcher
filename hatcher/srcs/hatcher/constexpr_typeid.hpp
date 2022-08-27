#pragma once

#include "string_view.hpp"

namespace hatcher
{

template <class T>
constexpr string_view constexpr_typeid();

} // namespace hatcher

#include "constexpr_typeid.ipp"
