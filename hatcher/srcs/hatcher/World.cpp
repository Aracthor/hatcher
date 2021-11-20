#include "World.hpp"

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
    m_entityIDRegistry.reset(new EntityIDRegistry());
}

World::~World() = default;

Entity World::CreateNewEntity()
{
    Entity entity = Entity(m_entityIDRegistry->GetNewID());

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