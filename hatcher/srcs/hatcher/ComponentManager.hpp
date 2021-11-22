#pragma once

#include <memory>
#include <optional>
#include <span>
#include <unordered_map>

#include "basic_types.hpp"

namespace hatcher
{
class Entity;
class EntityIDRegistry;
class IComponentList;

class ComponentManager
{
public:
    ComponentManager();
    ~ComponentManager();

    Entity CreateNewEntity();

    template <class Component>
    void AddComponentType();

    template <class Component>
    void AttachComponent(Entity entity, Component& component);

    template <class Component>
    std::span<const std::optional<Component>> GetComponents() const;

private:
    void AddEntities(int count);

    int m_maxEntityCount = 0;
    std::unique_ptr<EntityIDRegistry> m_entityIDRegistry;
    std::unordered_map<uint, IComponentList*> m_componentLists;
};

} // namespace hatcher

#include "ComponentManager.ipp"
