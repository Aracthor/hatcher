#pragma once

#include <memory>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace hatcher
{
class ComponentManager;
class Entity;
class EntityIDRegistry;
class Updater;

class World
{
public:
    World(const char* name);
    ~World();

    Entity CreateNewEntity();

    void AddRenderingUpdater(Updater* updater);

    template <class Component>
    void AddComponentType();
    template <class Component>
    void AttachComponent(Entity entity, Component& component);
    template <class Component>
    std::span<const std::optional<Component>> GetComponents() const;

    void UpdateRendering();

private:
    std::string m_name;
    std::unique_ptr<EntityIDRegistry> m_entityIDRegistry;
    int m_maxEntityCount = 0;

    std::unique_ptr<ComponentManager> m_componentManager;

    std::vector<std::unique_ptr<Updater>> m_renderingUpdaters;
};

} // namespace hatcher

#include "World.ipp"
