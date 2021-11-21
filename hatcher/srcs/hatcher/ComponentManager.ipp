#include "ComponentList.hpp"

#include "assert.hpp"
#include <iostream>

namespace hatcher
{

template <class Component>
void ComponentManager::AddComponentType()
{
    const uint key = Component::Key;
    HATCHER_ASSERT_MESSAGE(!m_componentLists.contains(key),
                           "Trying to register two times te same component type.");

    auto componentList = new IdentifiableComponentList<Component>();
    m_componentLists.emplace(key, componentList);
}

template <class Component>
void ComponentManager::AttachComponent(uint index, Component& component)
{
    using RealComponentList = IdentifiableComponentList<Component>*;

    const uint key = Component::Key;
    IComponentList* componentList = m_componentLists[key];
    RealComponentList realComponentList = reinterpret_cast<RealComponentList>(componentList);
    realComponentList->AttachComponent(index, component);
}

template <class Component>
std::span<const std::optional<Component>> ComponentManager::GetComponents() const
{
    using RealComponentList = const IdentifiableComponentList<Component>*;

    const uint key = Component::Key;
    // Why doesn't std::unordered_map have an operator[] returning a const value ??
    const IComponentList* componentList = m_componentLists.at(key);
    RealComponentList realComponentList = reinterpret_cast<RealComponentList>(componentList);
    return realComponentList->GetComponentList();
}

} // namespace hatcher
