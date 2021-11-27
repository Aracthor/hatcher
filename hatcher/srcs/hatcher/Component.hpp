#pragma once

#include "basic_types.hpp"
#include "constexpr_hash.hpp"

#include <typeinfo>

namespace hatcher
{

template <class T>
constexpr uint ComponentKey()
{
    return constexpr_hash(typeid(T).name());
}

} // namespace hatcher
