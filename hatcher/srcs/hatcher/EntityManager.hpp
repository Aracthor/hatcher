#pragma once

#include <vector>

#include "IEntityManager.hpp"
#include "span.hpp"
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

    span<const Entity> EntitiesToDelete() const { return span<const Entity>(m_entitiesToDelete); }
    void UpdateNewAndDeletedEntities();

    EntityEgg CreateNewEntity(const IEntityDescriptor* descriptor) override;
    EntityEgg CloneEntity(Entity entity) override;
    void DeleteEntity(Entity entity) override;

    void Save(ComponentSaver& saveLoader) override;
    void Load(ComponentLoader& saveLoader) override;

    ComponentManager* GetComponentManager() { return m_componentManager.get(); }
    ComponentManager* GetRenderingComponentManager() { return m_renderingComponentManager.get(); }

    ComponentManager* GetTemporaryComponentManager() { return m_temporaryComponentManager.get(); }
    ComponentManager* GetTemporaryRenderingComponentManager() { return m_temporaryRenderingComponentManager.get(); }

private:
    Entity CreateNewEntity();

    unique_ptr<EntityIDRegistry> m_entityIDRegistry;
    std::vector<Entity> m_entitiesToAdd;
    std::vector<Entity> m_entitiesToDelete;

    int m_maxEntityCount = 0;
    unique_ptr<ComponentManager> m_componentManager;
    unique_ptr<ComponentManager> m_renderingComponentManager;

    int m_maxTemporaryEntityCount = 0;
    unique_ptr<ComponentManager> m_temporaryComponentManager;
    unique_ptr<ComponentManager> m_temporaryRenderingComponentManager;
};

} // namespace hatcher
