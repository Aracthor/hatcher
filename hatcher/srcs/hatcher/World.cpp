#include "World.hpp"

#include "EntityManager.hpp"
#include "Updater.hpp"
#include "assert.hpp"

#include "Graphics/AbstractEventUpdater.hpp"
#include "Graphics/RenderUpdater.hpp"

#include <vector>

namespace hatcher
{

World::World(const char* name)
    : m_name(name)
{
    m_entityManager.reset(new EntityManager());
}

World::~World() = default;

void World::AddUpdater(Updater* updater)
{
    HATCHER_ASSERT(updater != nullptr);
    m_updaters.emplace_back(updater);
}

void World::AddRenderUpdater(RenderUpdater* updater)
{
    HATCHER_ASSERT(updater != nullptr);
    m_renderUpdaters.emplace_back(updater);
}

void World::SetEventUpdater(AbstractEventUpdater* updater)
{
    HATCHER_ASSERT(updater != nullptr);
    m_eventUpdater.reset(updater);
}

void World::Update()
{
    for (std::unique_ptr<Updater>& updater : m_updaters)
    {
        updater->Update(m_entityManager->GetComponentManager());
    }
}

void World::UpdateRendering(IFrameRenderer& frameRenderer, const Clock& clock)
{
    if (m_eventUpdater)
    {
        m_eventUpdater->PollEvents();
        m_eventUpdater->Update(m_entityManager.get(), m_entityManager->GetComponentManager(), clock,
                               frameRenderer);
    }

    for (std::unique_ptr<RenderUpdater>& renderUpdater : m_renderUpdaters)
    {
        renderUpdater->Update(m_entityManager->GetComponentManager(),
                              m_entityManager->GetRenderingComponentManager(), clock,
                              frameRenderer);
    }
}

} // namespace hatcher
