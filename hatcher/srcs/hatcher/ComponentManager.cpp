#include "ComponentManager.hpp"

#include "Entity.hpp"
#include "EntityIDRegistry.hpp"

namespace hatcher
{

ComponentManager::ComponentManager()
{
    m_entityIDRegistry.reset(new EntityIDRegistry());
}

ComponentManager::~ComponentManager()
{
    // TODO it would be smart to use smart pointers here...
    for (auto it : m_componentLists)
    {
        delete it.second;
    }
}

Entity ComponentManager::CreateNewEntity()
{
    Entity entity = Entity(m_entityIDRegistry->GetNewID());
    if (entity.ID() >= m_maxEntityCount)
    {
        const int newMaxEntityCount = entity.ID() + 1;
        const int entitiesAdded = newMaxEntityCount - m_maxEntityCount;
        m_maxEntityCount = entity.ID() + 1;
        AddEntities(entitiesAdded);
    }

    return entity;
}

void ComponentManager::AddEntities(int count)
{
    for (auto it : m_componentLists)
    {
        it.second->AddEntities(count);
    }
}

} // namespace hatcher
