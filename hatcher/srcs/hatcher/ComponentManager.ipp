#include "Component.hpp"
#include "ComponentList.hpp"
#include "Entity.hpp"
#include "assert.hpp"

#ifdef NDEBUG
#include <typeinfo>
#endif

namespace hatcher
{

template <class Component>
void ComponentManager::AddComponentType()
{
    const uint key = ComponentKey<Component>();
    HATCHER_ASSERT_MESSAGE(
        m_componentLists.find(key) == m_componentLists.end(),
        "Trying to register two times te same component type: " << typeid(Component).name());

    auto componentList = new IdentifiableComponentList<Component>();
    m_componentLists.emplace(key, componentList);
}

template <class Component>
void ComponentManager::AttachComponent(Entity entity, Component& component)
{
    using RealComponentList = IdentifiableComponentList<Component>*;

    const uint key = ComponentKey<Component>();
    HATCHER_ASSERT_MESSAGE(m_componentLists.find(key) != m_componentLists.end(),
                           "Requesting a missing component type: " << typeid(Component).name());
    IComponentList* componentList = m_componentLists[key];
    RealComponentList realComponentList = reinterpret_cast<RealComponentList>(componentList);
    realComponentList->AttachComponent(entity.ID(), component);
}

template <class Component>
span<const std::optional<Component>> ComponentManager::GetComponents() const
{
    using RealComponentList = const IdentifiableComponentList<Component>*;

    const uint key = ComponentKey<Component>();
    HATCHER_ASSERT_MESSAGE(m_componentLists.find(key) != m_componentLists.end(),
                           "Requesting a missing component type: " << typeid(Component).name());
    // Why doesn't std::unordered_map have an operator[] returning a const value ??
    const IComponentList* componentList = m_componentLists.at(key);
    RealComponentList realComponentList = reinterpret_cast<RealComponentList>(componentList);
    return realComponentList->GetComponentList();
}

template <class Component>
span<std::optional<Component>> ComponentManager::GetComponents()
{
    using RealComponentList = IdentifiableComponentList<Component>*;

    const uint key = ComponentKey<Component>();
    HATCHER_ASSERT_MESSAGE(m_componentLists.find(key) != m_componentLists.end(),
                           "Requesting a missing component type: " << typeid(Component).name());
    // Why doesn't std::unordered_map have an operator[] returning a const value ??
    IComponentList* componentList = m_componentLists.at(key);
    RealComponentList realComponentList = reinterpret_cast<RealComponentList>(componentList);
    return realComponentList->GetComponentList();
}

} // namespace hatcher
