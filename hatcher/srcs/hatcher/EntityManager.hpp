#pragma once

#include <memory>
#include <vector>

#include "IEntityManager.hpp"

namespace hatcher
{
class ComponentManager;
class Entity;
class EntityIDRegistry;
class IEntityDescriptor;

class EntityManager final : public IEntityManager
{
public:
    EntityManager();
    ~EntityManager();

    void StartUpdate();

    Entity CreateNewEntity(const IEntityDescriptor* descriptor) override;
    void DeleteEntity(Entity entity) override;
    bool IsEntityDeleted(Entity entity) const override;

    void Save(ComponentSaver& saveLoader) override;
    void Load(ComponentLoader& saveLoader) override;

    ComponentManager* GetComponentManager() { return m_componentManager.get(); }
    ComponentManager* GetRenderingComponentManager() { return m_renderingComponentManager.get(); }

private:
    void AddEntities(int count);

    int m_maxEntityCount = 0;
    std::unique_ptr<EntityIDRegistry> m_entityIDRegistry;
    std::vector<Entity> m_deletedEntities;

    std::unique_ptr<ComponentManager> m_componentManager;
    std::unique_ptr<ComponentManager> m_renderingComponentManager;
};

} // namespace hatcher
