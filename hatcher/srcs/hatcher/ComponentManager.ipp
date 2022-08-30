#include "Component.hpp"
#include "ComponentList.hpp"
#include "Entity.hpp"
#include "assert.hpp"
#include "checked_cast.hpp"

#ifdef NDEBUG
#include "constexpr_typeid.hpp"
#endif

namespace hatcher
{

template <class Component>
void ComponentManager::AddComponentType()
{
    constexpr uint key = ComponentKey<Component>();
    HATCHER_ASSERT_MESSAGE(
        m_componentLists.find(key) == m_componentLists.end(),
        "Trying to register two times te same component type: " << typeid(Component).name());

    auto componentList = new IdentifiableComponentList<Component>();
    m_componentLists.emplace(key, componentList);
}

template <class Component>
void ComponentManager::AddWorldComponent()
{
    constexpr uint key = ComponentKey<Component>();
    HATCHER_ASSERT_MESSAGE(
        m_worldComponents.find(key) == m_worldComponents.end(),
        "Trying to register two times te same world component: " << typeid(Component).name());

    m_worldComponents.emplace(key, std::make_unique<Component>());
}

template <class Component>
void ComponentManager::AttachComponent(Entity entity, Component& component)
{
    using RealComponentList = IdentifiableComponentList<Component>*;

    constexpr uint key = ComponentKey<Component>();
    HATCHER_ASSERT_MESSAGE(m_componentLists.find(key) != m_componentLists.end(),
                           "Requesting a missing component type: " << typeid(Component).name());
    IComponentList* componentList = m_componentLists[key].get();
    RealComponentList realComponentList = reinterpret_cast<RealComponentList>(componentList);
    realComponentList->AttachComponent(entity.ID(), component);
}

template <class Component>
ComponentReader<Component> ComponentManager::ReadComponents() const
{
    using RealComponentList = const IdentifiableComponentList<Component>*;

    constexpr uint key = ComponentKey<Component>();
    HATCHER_ASSERT_MESSAGE(
        m_componentLists.find(key) != m_componentLists.end(),
        "Requesting a missing component type: " << constexpr_typeid<Component>());
    // Why doesn't std::unordered_map have an operator[] returning a const value ??
    const IComponentList* componentList = m_componentLists.at(key).get();
    RealComponentList realComponentList = reinterpret_cast<RealComponentList>(componentList);
    return realComponentList->GetComponentList();
}

template <class Component>
ComponentWriter<Component> ComponentManager::WriteComponents()
{
    using RealComponentList = IdentifiableComponentList<Component>*;

    constexpr uint key = ComponentKey<Component>();
    HATCHER_ASSERT_MESSAGE(
        m_componentLists.find(key) != m_componentLists.end(),
        "Requesting a missing component type: " << constexpr_typeid<Component>());
    IComponentList* componentList = m_componentLists.at(key).get();
    RealComponentList realComponentList = reinterpret_cast<RealComponentList>(componentList);
    return realComponentList->GetComponentList();
}

template <class Component>
const Component* ComponentManager::ReadWorldComponent() const
{
    constexpr uint key = ComponentKey<Component>();
    HATCHER_ASSERT_MESSAGE(
        m_worldComponents.find(key) != m_worldComponents.end(),
        "Requesting a missing world component: " << constexpr_typeid<Component>());
    const IWorldComponent* component = m_worldComponents.at(key).get();
    return checked_cast<const Component*>(component);
}

template <class Component>
Component* ComponentManager::WriteWorldComponent()
{
    constexpr uint key = ComponentKey<Component>();
    HATCHER_ASSERT_MESSAGE(
        m_worldComponents.find(key) != m_worldComponents.end(),
        "Requesting a missing world component: " << constexpr_typeid<Component>());
    IWorldComponent* component = m_worldComponents.at(key).get();
    return checked_cast<Component*>(component);
}

} // namespace hatcher
