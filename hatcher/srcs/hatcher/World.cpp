#include "World.hpp"

#include "ComponentManager.hpp"
#include "Entity.hpp"
#include "EntityIDRegistry.hpp"
#include "Updater.hpp"
#include "assert.hpp"

#include <vector>

namespace hatcher
{

World::World(const char* name)
    : m_name(name)
{
    m_componentManager.reset(new ComponentManager());
    m_entityIDRegistry.reset(new EntityIDRegistry());
}

World::~World() = default;

Entity World::CreateNewEntity()
{
    Entity entity = Entity(m_entityIDRegistry->GetNewID());
    if (entity.ID() >= m_maxEntityCount)
    {
        const int newMaxEntityCount = entity.ID() + 1;
        const int entitiesAdded = newMaxEntityCount - m_maxEntityCount;
        m_maxEntityCount = entity.ID() + 1;
        m_componentManager->AddEntities(entitiesAdded);
    }

    return entity;
}

void World::AddRenderingUpdater(Updater* updater)
{
    HATCHER_ASSERT(updater != nullptr);
    m_renderingUpdaters.emplace_back(updater);
}

void World::UpdateRendering()
{
    for (std::unique_ptr<Updater>& updater : m_renderingUpdaters)
    {
        updater->Update(*this);
    }
}

} // namespace hatcher