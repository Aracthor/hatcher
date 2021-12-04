#pragma once

#include "hatcher/glm_pure.hpp"

namespace hatcher
{

template <glm::length_t L, typename T>
class Box
{
private:
    using Vec = glm::vec<L, T>;

public:
    Box() = default;
    Box(const Vec& point);
    Box(const Vec& min, const Vec& max);

    void AddPoint(const Vec& point);

    bool Contains(const Vec& point) const;
    bool Contains(const Box<L, T>& box) const;

    const Vec& Min() const { return m_min; }
    const Vec& Max() const { return m_max; }

private:
    glm::vec<L, T> m_min;
    glm::vec<L, T> m_max;
};

using Box2f = Box<2, float>;

} // namespace hatcher

#include "Box.ipp"
