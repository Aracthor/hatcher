#pragma once

#include <vector>

#include "IEntityManager.hpp"
#include "unique_ptr.hpp"

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

    void UpdateDeletedEntities();

    Entity CreateNewEntity(const IEntityDescriptor* descriptor) override;
    Entity CloneEntity(Entity entity) override;
    void DeleteEntity(Entity entity) override;

    void Save(ComponentSaver& saveLoader) override;
    void Load(ComponentLoader& saveLoader) override;

    ComponentManager* GetComponentManager() { return m_componentManager.get(); }
    ComponentManager* GetRenderingComponentManager() { return m_renderingComponentManager.get(); }

private:
    Entity CreateNewEntity();

    int m_maxEntityCount = 0;
    unique_ptr<EntityIDRegistry> m_entityIDRegistry;
    std::vector<Entity> m_entitiesToDelete;

    unique_ptr<ComponentManager> m_componentManager;
    unique_ptr<ComponentManager> m_renderingComponentManager;
};

} // namespace hatcher
