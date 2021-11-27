#pragma once

#include "hatcher/assert.hpp"
#include "hatcher/glm_pure.hpp"

namespace hatcher
{

template <glm::length_t L, typename T>
inline T angle(const glm::vec<L, T>& parU, const glm::vec<L, T>& parV)
{
    HATCHER_ASSERT(glm::normalize(parU) == parU);
    HATCHER_ASSERT(glm::normalize(parV) == parV);
    return ::acosf(glm::dot(parU, parV));
}

} // namespace hatcher
