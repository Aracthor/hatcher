#include <algorithm>

namespace hatcher
{

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
void Box<L, T>::AddPoint(const Vec& point)
{
    for (glm::length_t i = 0; i < L; i++)
    {
        m_min[i] = std::min(m_min[i], point[i]);
        m_max[i] = std::max(m_max[i], point[i]);
    }
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

} // namespace hatcher
