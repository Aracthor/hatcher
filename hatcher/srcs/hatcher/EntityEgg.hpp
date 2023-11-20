#pragma once

#include <optional>

#include "Entity.hpp"

namespace hatcher
{
class ComponentManager;

class EntityEgg
{
public:
    EntityEgg(Entity newEntity, Entity temporaryEntityID, ComponentManager* componentManager,
              ComponentManager* renderingComponentManager)
        : m_newEntity(newEntity)
        , m_temporaryEntityID(temporaryEntityID)
        , m_componentManager(componentManager)
        , m_renderingComponentManager(renderingComponentManager)
    {
    }

    Entity NewEntityID() const { return m_newEntity; }

    template <class Component>
    std::optional<Component>& GetComponent();

private:
    Entity m_newEntity;
    Entity m_temporaryEntityID;
    ComponentManager* m_componentManager;
    ComponentManager* m_renderingComponentManager;
};

} // namespace hatcher

#include "EntityEgg.ipp"
