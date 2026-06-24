#include <algorithm>
#include <limits>

namespace hatcher
{
template <std::size_t L, typename T>
constexpr int Box<L, T>::_pow2(std::size_t n)
{
    int result = 1;
    for (int i = 0; i < static_cast<int>(n); i++)
    {
        result *= 2;
    }
    return result;
}

template <std::size_t L, typename T>
constexpr Box<L, T>::Box()
    : m_min(std::numeric_limits<T>::max())
    , m_max(std::numeric_limits<T>::lowest())
{
}

template <std::size_t L, typename T>
constexpr Box<L, T>::Box(const VecType& point)
    : m_min(point)
    , m_max(point)
{
}

template <std::size_t L, typename T>
constexpr Box<L, T>::Box(const VecType& min, const VecType& max)
    : m_min(min)
    , m_max(max)
{
}

template <std::size_t L, typename T>
template <class Iterator>
constexpr Box<L, T>::Box(Iterator first, Iterator last)
{
    m_min = VecType(std::numeric_limits<T>::max());
    m_max = VecType(std::numeric_limits<T>::lowest());
    for (Iterator it = first; it != last; it++)
    {
        AddPoint(*it);
    }
}

template <std::size_t L, typename T>
void Box<L, T>::AddPoint(const VecType& point)
{
    for (std::size_t i = 0; i < L; i++)
    {
        m_min[i] = std::min(m_min[i], point[i]);
        m_max[i] = std::max(m_max[i], point[i]);
    }
}

template <std::size_t L, typename T>
void Box<L, T>::Add(const Box<L, T>& box)
{
    AddPoint(box.Min());
    AddPoint(box.Max());
}

template <std::size_t L, typename T>
Box<L, T> Box<L, T>::Translated(const VecType& translation) const
{
    return Box(m_min + translation, m_max + translation);
}

template <std::size_t L, typename T>
Box<L, T> Box<L, T>::Scaled(float scale) const
{
    return Box(m_min * scale, m_max * scale);
}

template <std::size_t L, typename T>
bool Box<L, T>::IsEmpty() const
{
    return m_min.x > m_max.x;
}

template <std::size_t L, typename T>
bool Box<L, T>::IsPoint() const
{
    return m_min == m_max;
}

template <std::size_t L, typename T>
bool Box<L, T>::Contains(const VecType& point) const
{
    for (std::size_t i = 0; i < L; i++)
    {
        if (m_min[i] > point[i] || m_max[i] < point[i])
            return false;
    }
    return true;
}

template <std::size_t L, typename T>
bool Box<L, T>::Contains(const Box<L, T>& box) const
{
    for (std::size_t i = 0; i < L; i++)
    {
        if (m_min[i] > box.m_min[i] || m_max[i] < box.m_max[i])
            return false;
    }
    return true;
}

template <std::size_t L, typename T>
bool Box<L, T>::Touches(const Box<L, T>& box) const
{
    for (std::size_t i = 0; i < L; i++)
    {
        if (m_min[i] > box.m_max[i] || m_max[i] < box.m_min[i])
            return false;
    }
    return true;
}

template <std::size_t L, typename T>
std::array<Vec<L, T>, Box<L, T>::CornerCount> Box<L, T>::GetCorners() const
{
    std::array<VecType, CornerCount> corners;
    for (std::size_t n = 0; n < L; n++)
    {
        for (int i = 0; i < CornerCount; i++)
            corners[i][n] = (i / _pow2(n)) % 2 == 0 ? m_min[n] : m_max[n];
    }
    return corners;
}

template <std::size_t L, typename T>
bool Box<L, T>::operator==(const Box& other) const
{
    return m_min == other.m_min && m_max == other.m_max;
}

template <std::size_t L, typename T>
bool Box<L, T>::operator!=(const Box& other) const
{
    return !operator==(other);
}

} // namespace hatcher
