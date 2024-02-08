#pragma once

#include "constexpr_hash.hpp"
#include "constexpr_typeid.hpp"

namespace hatcher
{

template <class T>
constexpr uint ClassKey()
{
    constexpr string_view className = constexpr_typeid<T>();
    return constexpr_hash<className.size>(className.data);
}

} // namespace hatcher
