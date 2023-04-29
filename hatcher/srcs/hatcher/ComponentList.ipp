#include "assert.hpp"

#include "ISaveLoader.hpp"

namespace hatcher
{

template <class Component>
void IdentifiableComponentList<Component>::AddEntities(int count)
{
    m_components.resize(m_components.size() + count);
}

template <class Component>
void IdentifiableComponentList<Component>::RemoveEntity(uint index)
{
    m_components[index] = {};
}

template <class Component>
void IdentifiableComponentList<Component>::ClearEntities()
{
    m_components.clear();
}

template <class Component>
bool IdentifiableComponentList<Component>::HasComponent(uint index) const
{
    HATCHER_ASSERT(index < m_components.size());
    return m_components[index].has_value();
}

template <class Component>
void IdentifiableComponentList<Component>::SaveLoad(uint index, ISaveLoader& saveLoader)
{
    HATCHER_ASSERT(index < m_components.size());
    saveLoader << m_components[index];
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