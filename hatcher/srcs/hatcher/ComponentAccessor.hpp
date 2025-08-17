#pragma once

#include <optional>

#include "ComponentManager.hpp"
#include "span.hpp"

namespace hatcher
{

class ComponentManager;

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

class ComponentAccessor
{
public:
    explicit ComponentAccessor(ComponentManager* componentManager)
        : m_componentManager(componentManager)
    {
    }

    int Count() const { return m_componentManager->Count(); }

    template <class Component>
    ComponentReader<Component> ReadComponents() const;
    template <class Component>
    ComponentWriter<Component> WriteComponents();

    template <class Component>
    const Component* ReadWorldComponent() const;
    template <class Component>
    Component* WriteWorldComponent();

private:
    ComponentManager* m_componentManager;
};

} // namespace hatcher

#include "ComponentAccessor.ipp"