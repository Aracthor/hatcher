#include <algorithm>
#include <limits>

namespace hatcher
{
template <glm::length_t L, typename T>
constexpr int Box<L, T>::_pow2(glm::length_t n)
{
    int result = 1;
    for (int i = 0; i < static_cast<int>(n); i++)
    {
        result *= 2;
    }
    return result;
}

template <glm::length_t L, typename T>
Box<L, T>::Box(const Vec& point)
    : m_min(point)
    , m_max(point)
{
}

template <glm::length_t L, typename T>
Box<L, T>::Box(const Vec& min, const Vec& max)
    : m_min(min)
    , m_max(max)
{
}

template <glm::length_t L, typename T>
template <class Iterator>
Box<L, T>::Box(Iterator first, Iterator last)
{
    m_min = Vec(std::numeric_limits<T>::max());
    m_max = Vec(std::numeric_limits<T>::lowest());
    for (Iterator it = first; it != last; it++)
    {
        AddPoint(*it);
    }
}

template <glm::length_t L, typename T>
void Box<L, T>::AddPoint(const Vec& point)
{
    for (glm::length_t i = 0; i < L; i++)
    {
        m_min[i] = std::min(m_min[i], point[i]);
        m_max[i] = std::max(m_max[i], point[i]);
    }
}

template <glm::length_t L, typename T>
Box<L, T> Box<L, T>::Translated(const Vec& translation) const
{
    return Box(m_min + translation, m_max + translation);
}

template <glm::length_t L, typename T>
bool Box<L, T>::Contains(const Vec& point) const
{
    for (glm::length_t i = 0; i < L; i++)
    {
        if (m_min[i] > point[i] || m_max[i] < point[i])
            return false;
    }
    return true;
}

template <glm::length_t L, typename T>
bool Box<L, T>::Contains(const Box<L, T>& box) const
{
    for (glm::length_t i = 0; i < L; i++)
    {
        if (m_min[i] > box.m_min[i] || m_max[i] < box.m_max[i])
            return false;
    }
    return true;
}

template <glm::length_t L, typename T>
bool Box<L, T>::Touches(const Box<L, T>& box) const
{
    for (glm::length_t i = 0; i < L; i++)
    {
        if (m_min[i] > box.m_max[i] || m_max[i] < box.m_min[i])
            return false;
    }
    return true;
}

template <glm::length_t L, typename T>
std::array<glm::vec<L, T>, Box<L, T>::CornerCount> Box<L, T>::GetCorners() const
{
    std::array<Vec, CornerCount> corners;
    for (glm::length_t n = 0; n < L; n++)
    {
        for (int i = 0; i < CornerCount; i++)
            corners[i][n] = (i / _pow2(n)) % 2 == 0 ? m_min[n] : m_max[n];
    }
    return corners;
}

template <glm::length_t L, typename T>
bool Box<L, T>::operator==(const Box& other) const
{
    return m_min == other.m_min && m_max == other.m_max;
}

template <glm::length_t L, typename T>
bool Box<L, T>::operator!=(const Box& other) const
{
    return !operator==(other);
}

} // namespace hatcher
