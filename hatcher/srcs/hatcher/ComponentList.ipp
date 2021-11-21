#include "assert.hpp"

#include <iostream>
namespace hatcher
{

template <class Component>
void IdentifiableComponentList<Component>::AddEntities(int count)
{
    m_components.resize(m_components.size() + count);
}

template <class Component>
void IdentifiableComponentList<Component>::AttachComponent(uint index, Component& component)
{
    HATCHER_ASSERT(index < m_components.size());
    m_components[index] = component;
}

template <class Component>
std::span<const std::optional<Component>>
IdentifiableComponentList<Component>::GetComponentList() const
{
    return std::span<const std::optional<Component>>(m_components);
}

} // namespace hatcher