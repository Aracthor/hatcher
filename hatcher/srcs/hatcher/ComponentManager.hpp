#pragma once

#include <memory>
#include <optional>
#include <unordered_map>

#include "Entity.hpp"
#include "basic_types.hpp"
#include "span.hpp"

namespace hatcher
{
class IComponentList;

template <class Component>
using ComponentReader = span<const std::optional<Component>>;
template <class Component>
using ComponentWriter = span<std::optional<Component>>;

class ComponentManager
{
public:
    ComponentManager();
    ~ComponentManager();

    void AddEntities(int count);

    int Count() const { return m_entityCount; }

    template <class Component>
    void AddComponentType();

    template <class Component>
    void AttachComponent(Entity entity, Component& component);

    template <class Component>
    ComponentReader<Component> ReadComponents() const;

    template <class Component>
    ComponentWriter<Component> WriteComponents();

private:
    int m_entityCount = 0;
    std::unordered_map<uint, std::unique_ptr<IComponentList>> m_componentLists;
};

} // namespace hatcher

#include "ComponentManager.ipp"
