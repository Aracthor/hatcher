#include "EntityManager.hpp"

#include "ComponentManager.hpp"
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

} // namespace hatcher
