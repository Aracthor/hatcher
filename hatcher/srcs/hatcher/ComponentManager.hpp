#pragma once

#include <memory>
#include <optional>
#include <span>
#include <unordered_map>

#include "basic_types.hpp"

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
    void AttachComponent(uint index, Component& component);

    template <class Component>
    std::span<const std::optional<Component>> GetComponents() const;

private:
    std::unordered_map<uint, IComponentList*> m_componentLists;
};

} // namespace hatcher

#include "ComponentManager.ipp"
