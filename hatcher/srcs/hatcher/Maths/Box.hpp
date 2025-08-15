#pragma once

#include <array>

#include "glm_pure.hpp"

namespace hatcher
{

template <glm::length_t L, typename T>
class Box
{
private:
    constexpr static int _pow2(glm::length_t n);
    const static int CornerCount = _pow2(L);

    using Vec = glm::vec<L, T>;

public:
    constexpr Box();
    constexpr Box(const Vec& point);
    constexpr Box(const Vec& min, const Vec& max);
    template <class Iterator>
    constexpr Box(Iterator first, Iterator last);

    void AddPoint(const Vec& point);
    void Add(const Box<L, T>& box);
    Box Translated(const Vec& translation) const;
    Box Scaled(float scale) const;

    bool IsEmpty() const;
    bool IsPoint() const;
    bool Contains(const Vec& point) const;
    bool Contains(const Box<L, T>& box) const;
    bool Touches(const Box<L, T>& box) const;

    const Vec& Min() const { return m_min; }
    const Vec& Max() const { return m_max; }

    Vec& Min() { return m_min; }
    Vec& Max() { return m_max; }

    void SetMin(const Vec& min) { m_min = min; }
    void SetMax(const Vec& max) { m_max = max; }

    Vec Extents() const { return m_max - m_min; }
    Vec Center() const { return (m_max + m_min) / static_cast<T>(2); }
    std::array<Vec, CornerCount> GetCorners() const;

    bool operator==(const Box& other) const;
    bool operator!=(const Box& other) const;

private:
    glm::vec<L, T> m_min;
    glm::vec<L, T> m_max;
};

using Box2f = Box<2, float>;
using Box3f = Box<3, float>;

} // namespace hatcher

#include "Box.ipp"
