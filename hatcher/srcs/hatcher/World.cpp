#include "World.hpp"

#include <iostream>
#include <vector>

#include "CommandManager.hpp"
#include "ComponentRegisterer.hpp"
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

std::vector<const IComponentTypeCreator*>& ComponentTypeCreators()
{
    static std::vector<const IComponentTypeCreator*> creators;
    return creators;
}

std::vector<const IComponentTypeCreator*>& RenderComponentTypeCreators()
{
    static std::vector<const IComponentTypeCreator*> creators;
    return creators;
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

void RegisterComponentTypeCreator(const IComponentTypeCreator* creator)
{
    ComponentTypeCreators().push_back(creator);
}

void RegisterRenderComponentTypeCreator(const IComponentTypeCreator* creator)
{
    RenderComponentTypeCreators().push_back(creator);
}

World::World(const char* name)
    : m_name(name)
{
    m_entityManager.reset(new EntityManager());
    for (auto creator : ComponentTypeCreators())
    {
        creator->CreateComponentType(m_entityManager->GetComponentManager());
    }
    for (auto creator : UpdaterCreators())
    {
        m_updaters.emplace_back(creator->Create());
    }
    m_commandManager = std::make_unique<CommandManager>();
}

World::~World() = default;

void World::CreateRenderUpdaters(const IRendering* rendering)
{
    m_eventUpdater = std::make_unique<EventUpdater>();
    for (auto creator : RenderComponentTypeCreators())
    {
        creator->CreateComponentType(m_entityManager->GetRenderingComponentManager());
    }
    for (auto creator : RenderUpdaterCreators())
    {
        m_renderUpdaters.emplace_back(creator->Create(rendering, m_eventUpdater.get()));
    }
}

void World::Update()
{
    m_entityManager->StartUpdate();
    for (std::unique_ptr<Updater>& updater : m_updaters)
    {
        updater->Update(m_entityManager->GetComponentManager());
    }
    m_commandManager->ExecuteCommands(m_entityManager.get(), m_entityManager->GetComponentManager(),
                                      m_entityManager->GetRenderingComponentManager());
}

void World::UpdateFromEvents(span<const SDL_Event> events, IApplication* application, IFrameRenderer& frameRenderer)
{
    if (m_eventUpdater)
    {
        m_eventUpdater->ProcessEvents(events, application, m_commandManager.get(),
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
