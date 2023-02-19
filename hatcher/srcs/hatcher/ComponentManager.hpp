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
class ISaveLoader;
class IWorldComponent;

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

    void SaveLoad(ISaveLoader& saveLoader);

    int Count() const { return m_entityCount; }

    template <class Component>
    void AddComponentType();

    template <class Component>
    void AddWorldComponent();

    template <class Component>
    void AttachComponent(Entity entity, Component& component);

    template <class Component>
    ComponentReader<Component> ReadComponents() const;
    template <class Component>
    ComponentWriter<Component> WriteComponents();

    template <class Component>
    const Component* ReadWorldComponent() const;
    template <class Component>
    Component* WriteWorldComponent();

private:
    int m_entityCount = 0;
    std::unordered_map<uint, std::unique_ptr<IComponentList>> m_componentLists;
    std::unordered_map<uint, std::unique_ptr<IWorldComponent>> m_worldComponents;
};

} // namespace hatcher

#include "ComponentManager.ipp"
