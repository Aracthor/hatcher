#pragma once

#include <memory>

#include "IEntityManager.hpp"

namespace hatcher
{
class ComponentManager;
class Entity;
class EntityIDRegistry;

class EntityManager final : public IEntityManager
{
public:
    EntityManager();
    ~EntityManager();

    Entity CreateNewEntity() override;

    void Save(ISaveLoader& saveLoader) override;
    void Load(ISaveLoader& saveLoader) override;

    ComponentManager* GetComponentManager() { return m_componentManager.get(); }
    ComponentManager* GetRenderingComponentManager() { return m_renderingComponentManager.get(); }

private:
    void AddEntities(int count);

    int m_maxEntityCount = 0;
    std::unique_ptr<EntityIDRegistry> m_entityIDRegistry;

    std::unique_ptr<ComponentManager> m_componentManager;
    std::unique_ptr<ComponentManager> m_renderingComponentManager;
};

} // namespace hatcher
