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
std::vector<CreateUpdaterFunction*>& UpdaterCreators()
{
    static std::vector<CreateUpdaterFunction*> updaterCreators;
    return updaterCreators;
}

std::vector<CreateRenderUpdaterFunction*>& RenderUpdaterCreators()
{
    static std::vector<CreateRenderUpdaterFunction*> updaterCreators;
    return updaterCreators;
}
} // namespace

int RegisterUpdater(CreateUpdaterFunction* createFunction)
{
    auto& updaterCreators = UpdaterCreators();
    updaterCreators.push_back(createFunction);
    return 0;
}

int RegisterRenderUpdater(CreateRenderUpdaterFunction* createFunction)
{
    auto& renderUpdaterCreators = RenderUpdaterCreators();
    renderUpdaterCreators.push_back(createFunction);
    return 0;
}

World::World(const char* name)
    : m_name(name)
{
    m_entityManager.reset(new EntityManager());
    for (auto entry : UpdaterCreators())
    {
        m_updaters.emplace_back(entry());
    }
    m_eventUpdater = std::make_unique<EventUpdater>();
}

World::~World() = default;

void World::CreateRenderUpdaters(const IRendering* rendering)
{
    for (auto entry : RenderUpdaterCreators())
    {
        m_renderUpdaters.emplace_back(entry(rendering, m_eventUpdater.get()));
    }
}

void World::Update()
{
    for (std::unique_ptr<Updater>& updater : m_updaters)
    {
        updater->Update(m_entityManager->GetComponentManager());
    }
}

void World::UpdateRendering(IApplication* application, IFrameRenderer& frameRenderer)
{
    if (m_eventUpdater)
    {
        m_eventUpdater->PollEvents(application, m_entityManager.get(), m_entityManager->GetComponentManager(),
                                   m_entityManager->GetRenderingComponentManager(), frameRenderer);
    }

    for (std::unique_ptr<RenderUpdater>& renderUpdater : m_renderUpdaters)
    {
        renderUpdater->Update(m_entityManager->GetComponentManager(), m_entityManager->GetRenderingComponentManager(),
                              frameRenderer);
    }
}

} // namespace hatcher
