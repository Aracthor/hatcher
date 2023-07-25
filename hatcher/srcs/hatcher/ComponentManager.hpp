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
class ComponentReader
{
public:
    inline ComponentReader(span<const std::optional<Component>>&& data)
        : m_data(data)
    {
    }

    inline const std::optional<Component>& operator[](int index) const { return m_data[index]; }
    inline const std::optional<Component>& operator[](Entity entity) const { return m_data[entity.ID()]; }

private:
    span<const std::optional<Component>> m_data;
};

template <class Component>
class ComponentWriter
{
public:
    inline ComponentWriter(span<std::optional<Component>>&& data)
        : m_data(data)
    {
    }

    inline const std::optional<Component>& operator[](int index) const { return m_data[index]; }
    inline std::optional<Component>& operator[](int index) { return m_data[index]; }
    inline const std::optional<Component>& operator[](Entity entity) const { return m_data[entity.ID()]; }
    inline std::optional<Component>& operator[](Entity entity) { return m_data[entity.ID()]; }

private:
    span<std::optional<Component>> m_data;
};

class ComponentManager
{
public:
    ComponentManager();
    ~ComponentManager();

    void AddEntities(int count);
    void RemoveEntity(Entity entity);
    void ClearEntities();

    void Save(ISaveLoader& saver);
    void Load(ISaveLoader& saver);
    void LoadEntityComponents(ISaveLoader& loader, int entityID);

    int Count() const { return m_entityCount; }

    template <class Component>
    void AddComponentType();

    template <class Component>
    void AddWorldComponent();

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
