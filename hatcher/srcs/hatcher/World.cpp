#include "World.hpp"

#include "ComponentManager.hpp"
#include "Updater.hpp"
#include "assert.hpp"

#include <vector>

namespace hatcher
{

World::World(const char* name)
    : m_name(name)
{
    m_componentManager.reset(new ComponentManager());
}

World::~World() = default;

void World::AddRenderingUpdater(Updater* updater)
{
    HATCHER_ASSERT(updater != nullptr);
    m_renderingUpdaters.emplace_back(updater);
}

void World::UpdateRendering()
{
    for (std::unique_ptr<Updater>& updater : m_renderingUpdaters)
    {
        updater->Update(m_componentManager.get());
    }
}

} // namespace hatcher
