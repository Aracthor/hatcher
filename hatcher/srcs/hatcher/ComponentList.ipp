#include "assert.hpp"

#include "DataLoader.hpp"
#include "DataSaver.hpp"
#include "checked_cast.hpp"

namespace hatcher
{

template <class Component>
void IdentifiableComponentList<Component>::AddEntities(int count)
{
    m_components.resize(m_components.size() + count);
}

template <class Component>
void IdentifiableComponentList<Component>::CopyEntity(const IComponentList* other, int indexDest, int indexSrc)
{
    using Self = IdentifiableComponentList<Component>;
    const Self* otherList = checked_cast<const Self*>(other);
    m_components[indexDest] = otherList->m_components[indexSrc];
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
void IdentifiableComponentList<Component>::CreateComponent(uint index)
{
    HATCHER_ASSERT(index < m_components.size());
    m_components[index] = Component();
}

template <class Component>
void IdentifiableComponentList<Component>::Save(uint index, DataSaver& saver)
{
    HATCHER_ASSERT(index < m_components.size());
    HATCHER_ASSERT(m_components[index].has_value());
    saver << *m_components[index];
}

template <class Component>
void IdentifiableComponentList<Component>::Load(uint index, DataLoader& loader)
{
    HATCHER_ASSERT(index < m_components.size());
    HATCHER_ASSERT(m_components[index].has_value());
    loader >> *m_components[index];
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