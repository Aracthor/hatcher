#include "ComponentManager.hpp"
#include "Entity.hpp"

namespace hatcher
{

template <class Component>
void World::AddComponentType()
{
    m_componentManager->AddComponentType<Component>();
}

template <class Component>
void World::AttachComponent(Entity entity, Component& component)
{
    m_componentManager->AttachComponent(entity.ID(), component);
}

template <class Component>
std::span<const std::optional<Component>> World::GetComponents() const
{
    return m_componentManager->GetComponents<Component>();
}

} // namespace hatcher
