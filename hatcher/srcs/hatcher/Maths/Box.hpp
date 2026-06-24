#pragma once

#include <array>

#include "Vec.hpp"

namespace hatcher
{

template <std::size_t L, typename T>
class Box
{
private:
    constexpr static int _pow2(std::size_t n);
    const static int CornerCount = _pow2(L);

    using VecType = Vec<L, T>;

public:
    constexpr Box();
    constexpr Box(const VecType& point);
    constexpr Box(const VecType& min, const VecType& max);
    template <class Iterator>
    constexpr Box(Iterator first, Iterator last);

    void AddPoint(const VecType& point);
    void Add(const Box<L, T>& box);
    Box Translated(const VecType& translation) const;
    Box Scaled(float scale) const;

    bool IsEmpty() const;
    bool IsPoint() const;
    bool Contains(const VecType& point) const;
    bool Contains(const Box<L, T>& box) const;
    bool Touches(const Box<L, T>& box) const;

    const VecType& Min() const { return m_min; }
    const VecType& Max() const { return m_max; }

    VecType& Min() { return m_min; }
    VecType& Max() { return m_max; }

    void SetMin(const VecType& min) { m_min = min; }
    void SetMax(const VecType& max) { m_max = max; }

    VecType Extents() const { return m_max - m_min; }
    VecType Center() const { return (m_max + m_min) / static_cast<T>(2); }
    std::array<VecType, CornerCount> GetCorners() const;

    bool operator==(const Box& other) const;
    bool operator!=(const Box& other) const;

private:
    VecType m_min;
    VecType m_max;
};

using Box2i = Box<2, int>;
using Box2f = Box<2, float>;
using Box3f = Box<3, float>;

} // namespace hatcher

#include "Box.ipp"
