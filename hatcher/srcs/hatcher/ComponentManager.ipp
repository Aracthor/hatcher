#include "ClassKey.hpp"
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
    constexpr uint key = ClassKey<Component>();
    HATCHER_ASSERT_MESSAGE(m_componentLists.find(key) == m_componentLists.end(),
                           "Trying to register two times te same component type: " << typeid(Component).name());

    auto componentList = new IdentifiableComponentList<Component>();
    m_componentLists.emplace(key, componentList);
}

template <class Component>
void ComponentManager::AddWorldComponent()
{
    constexpr uint key = ClassKey<Component>();
    HATCHER_ASSERT_MESSAGE(m_worldComponents.find(key) == m_worldComponents.end(),
                           "Trying to register two times te same world component: " << typeid(Component).name());

    m_worldComponents.emplace(key, make_unique<Component>());
}

template <class Component>
const IComponentList* ComponentManager::GetComponentList() const
{
    constexpr uint key = ClassKey<Component>();
    HATCHER_ASSERT_MESSAGE(m_componentLists.find(key) != m_componentLists.end(),
                           "Requesting a missing component type: " << constexpr_typeid<Component>());
    return m_componentLists.at(key).get();
}

template <class Component>
IComponentList* ComponentManager::GetComponentList()
{
    constexpr uint key = ClassKey<Component>();
    HATCHER_ASSERT_MESSAGE(m_componentLists.find(key) != m_componentLists.end(),
                           "Requesting a missing component type: " << constexpr_typeid<Component>());
    return m_componentLists.at(key).get();
}

template <class Component>
const IWorldComponent* ComponentManager::GetWorldComponent() const
{
    constexpr uint key = ClassKey<Component>();
    HATCHER_ASSERT_MESSAGE(m_worldComponents.find(key) != m_worldComponents.end(),
                           "Requesting a missing world component: " << constexpr_typeid<Component>());
    return m_worldComponents.at(key).get();
}

template <class Component>
IWorldComponent* ComponentManager::GetWorldComponent()
{
    constexpr uint key = ClassKey<Component>();
    HATCHER_ASSERT_MESSAGE(m_worldComponents.find(key) != m_worldComponents.end(),
                           "Requesting a missing world component: " << constexpr_typeid<Component>());
    return m_worldComponents.at(key).get();
}

} // namespace hatcher
