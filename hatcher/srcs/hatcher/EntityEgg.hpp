#pragma once

#include <optional>

#include "Entity.hpp"

namespace hatcher
{
class ComponentAccessor;

class EntityEgg
{
public:
    EntityEgg(Entity newEntity, Entity temporaryEntityID, ComponentAccessor* componentAccessor,
              ComponentAccessor* renderingComponentAccessor)
        : m_newEntity(newEntity)
        , m_temporaryEntityID(temporaryEntityID)
        , m_componentAccessor(componentAccessor)
        , m_renderingComponentAccessor(renderingComponentAccessor)
    {
    }

    Entity NewEntityID() const { return m_newEntity; }

    template <class Component>
    std::optional<Component>& GetComponent();

    template <class Component>
    std::optional<Component>& GetRenderingComponent();

private:
    Entity m_newEntity;
    Entity m_temporaryEntityID;
    ComponentAccessor* m_componentAccessor;
    ComponentAccessor* m_renderingComponentAccessor;
};

} // namespace hatcher

#include "EntityEgg.ipp"
