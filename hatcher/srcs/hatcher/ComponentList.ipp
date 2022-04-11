#include "assert.hpp"

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
span<const std::optional<Component>> IdentifiableComponentList<Component>::GetComponentList() const
{
    return span<const std::optional<Component>>(m_components);
}

template <class Component>
span<std::optional<Component>> IdentifiableComponentList<Component>::GetComponentList()
{
    return span<std::optional<Component>>(m_components);
}

} // namespace hatcher