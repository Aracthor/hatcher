#pragma once

#include "basic_types.hpp"
#include "constexpr_hash.hpp"

namespace hatcher
{

template <typename T, size_t N = sizeof(T)>
constexpr uint ComponentKeyFromName(T name)
{
    return constexpr_hash(name);
}

} // namespace hatcher
