#include "World.hpp"

#include <iostream>
#include <vector>

#include "EntityManager.hpp"
#include "Updater.hpp"
#include "assert.hpp"

#include "Graphics/EventUpdater.hpp"
#include "Graphics/RenderUpdater.hpp"

namespace hatcher
{

namespace
{
std::vector<const IUpdaterCreator*>& UpdaterCreators()
{
    static std::vector<const IUpdaterCreator*> updaterCreators;
    return updaterCreators;
}

std::vector<const IRenderUpdaterCreator*>& RenderUpdaterCreators()
{
    static std::vector<const IRenderUpdaterCreator*> renderUpdaterCreators;
    return renderUpdaterCreators;
}
} // namespace

void RegisterUpdater(const IUpdaterCreator* creator)
{
    UpdaterCreators().push_back(creator);
}

void RegisterRenderUpdater(const IRenderUpdaterCreator* creator)
{
    RenderUpdaterCreators().push_back(creator);
}

World::World(const char* name)
    : m_name(name)
{
    m_entityManager.reset(new EntityManager());
    for (auto creator : UpdaterCreators())
    {
        m_updaters.emplace_back(creator->Create());
    }
    m_eventUpdater = std::make_unique<EventUpdater>();
}

World::~World() = default;

void World::CreateRenderUpdaters(const IRendering* rendering)
{
    for (auto creator : RenderUpdaterCreators())
    {
        m_renderUpdaters.emplace_back(creator->Create(rendering, m_eventUpdater.get()));
    }
}

void World::Update()
{
    for (std::unique_ptr<Updater>& updater : m_updaters)
    {
        updater->Update(m_entityManager->GetComponentManager());
    }
}

void World::UpdateFromEvents(span<const SDL_Event> events, IApplication* application, IFrameRenderer& frameRenderer)
{
    if (m_eventUpdater)
    {
        m_eventUpdater->ProcessEvents(events, application, m_entityManager.get(),
                                      m_entityManager->GetComponentManager(),
                                      m_entityManager->GetRenderingComponentManager(), frameRenderer);
    }
}

void World::UpdateRendering(IFrameRenderer& frameRenderer)
{
    for (std::unique_ptr<RenderUpdater>& renderUpdater : m_renderUpdaters)
    {
        renderUpdater->Update(m_entityManager->GetComponentManager(), m_entityManager->GetRenderingComponentManager(),
                              frameRenderer);
    }
}

} // namespace hatcher
