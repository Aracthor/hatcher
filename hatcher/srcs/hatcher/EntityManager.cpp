#include "EntityManager.hpp"

#include "ComponentLoader.hpp"
#include "ComponentManager.hpp"
#include "ComponentSaver.hpp"
#include "Entity.hpp"
#include "EntityIDRegistry.hpp"

namespace hatcher
{

EntityManager::EntityManager()
{
    m_entityIDRegistry.reset(new EntityIDRegistry());

    m_componentManager = std::make_unique<ComponentManager>();
    m_renderingComponentManager = std::make_unique<ComponentManager>();
}

EntityManager::~EntityManager() = default;

void EntityManager::StartUpdate()
{
    m_deletedEntities.clear();
}

Entity EntityManager::CreateNewEntity()
{
    Entity entity = Entity(m_entityIDRegistry->GetNewID());
    if (entity.ID() >= m_maxEntityCount)
    {
        const int newMaxEntityCount = entity.ID() + 1;
        const int entitiesAdded = newMaxEntityCount - m_maxEntityCount;
        m_maxEntityCount = entity.ID() + 1;
        m_componentManager->AddEntities(entitiesAdded);
        if (m_renderingComponentManager)
            m_renderingComponentManager->AddEntities(entitiesAdded);
    }

    return entity;
}

void EntityManager::DeleteEntity(Entity entity)
{
    m_deletedEntities.push_back(entity);
    m_componentManager->RemoveEntity(entity);
    if (m_renderingComponentManager)
        m_renderingComponentManager->RemoveEntity(entity);
    m_entityIDRegistry->UnregisterEntityID(entity.ID());
}

bool EntityManager::IsEntityDeleted(Entity entity) const
{
    return std::find(m_deletedEntities.begin(), m_deletedEntities.end(), entity) != m_deletedEntities.end();
}

void EntityManager::Save(ComponentSaver& saver)
{
    int entityCount = m_entityIDRegistry->EntityCount();
    saver << entityCount;
    saver.separator('\n');
    m_componentManager->Save(saver);
    m_renderingComponentManager->Save(saver);
}

void EntityManager::Load(ComponentLoader& loader)
{
    int entityCount;
    loader << entityCount;
    loader.separator('\n');
    m_maxEntityCount = entityCount;
    m_entityIDRegistry->ResetEntityCount(entityCount);
    m_componentManager->ClearEntities();
    m_componentManager->AddEntities(entityCount);
    m_componentManager->Load(loader);
    m_renderingComponentManager->ClearEntities();
    m_renderingComponentManager->AddEntities(entityCount);
    m_renderingComponentManager->Load(loader);
}

} // namespace hatcher
