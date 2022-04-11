#pragma once

#include <optional>
#include <unordered_map>

#include "Entity.hpp"
#include "basic_types.hpp"
#include "span.hpp"

namespace hatcher
{
class IComponentList;

class ComponentManager
{
public:
    ComponentManager();
    ~ComponentManager();

    void AddEntities(int count);

    template <class Component>
    void AddComponentType();

    template <class Component>
    void AttachComponent(Entity entity, Component& component);

    template <class Component>
    span<const std::optional<Component>> GetComponents() const;

    template <class Component>
    span<std::optional<Component>> GetComponents();

private:
    std::unordered_map<uint, IComponentList*> m_componentLists;
};

} // namespace hatcher

#include "ComponentManager.ipp"
