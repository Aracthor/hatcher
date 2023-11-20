#include "World.hpp"

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

using TComponentCreators = std::vector<const IComponentTypeCreator*>[EComponentList::COUNT];

TComponentCreators& ComponentTypeCreators()
{
    static TComponentCreators creators;
    return creators;
}

TComponentCreators& WorldComponentTypeCreators()
{
    static TComponentCreators creators;
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

void RegisterComponentTypeCreator(const IComponentTypeCreator* creator, EComponentList::Type type)
{
    ComponentTypeCreators()[type].push_back(creator);
}

void RegisterWorldComponentTypeCreator(const IComponentTypeCreator* creator, EComponentList::Type type)
{
    WorldComponentTypeCreators()[type].push_back(creator);
}

World::World()
{
    m_entityManager = make_unique<EntityManager>();
    for (auto creator : ComponentTypeCreators()[EComponentList::Gameplay])
    {
        creator->CreateComponentType(m_entityManager->GetComponentManager());
        creator->CreateComponentType(m_entityManager->GetTemporaryComponentManager());
    }
    for (auto creator : WorldComponentTypeCreators()[EComponentList::Gameplay])
    {
        creator->CreateComponentType(m_entityManager->GetComponentManager());
    }
    for (auto creator : UpdaterCreators())
    {
        m_updaters.emplace_back(creator->Create());
    }
    m_commandManager = make_unique<CommandManager>();
}

World::~World() = default;

void World::CreateRenderUpdaters(const IRendering* rendering)
{
    m_eventUpdater = make_unique<EventUpdater>();
    for (auto creator : ComponentTypeCreators()[EComponentList::Rendering])
    {
        creator->CreateComponentType(m_entityManager->GetRenderingComponentManager());
        creator->CreateComponentType(m_entityManager->GetTemporaryRenderingComponentManager());
    }
    for (auto creator : WorldComponentTypeCreators()[EComponentList::Rendering])
    {
        creator->CreateComponentType(m_entityManager->GetRenderingComponentManager());
    }
    for (auto creator : RenderUpdaterCreators())
    {
        RenderUpdater* renderUpdater = creator->Create(rendering);
        m_renderUpdaters.emplace_back(renderUpdater);
        m_eventUpdater->RegisterListener(renderUpdater);
    }
}

void World::Update()
{
    for (unique_ptr<Updater>& updater : m_updaters)
    {
        updater->Update(m_entityManager.get(), m_entityManager->GetComponentManager());
    }
    m_commandManager->ExecuteCommands(m_entityManager.get(), m_entityManager->GetComponentManager(),
                                      m_entityManager->GetRenderingComponentManager());
    m_entityManager->UpdateNewAndDeletedEntities();
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
    for (unique_ptr<RenderUpdater>& renderUpdater : m_renderUpdaters)
    {
        renderUpdater->Update(m_entityManager->GetComponentManager(), m_entityManager->GetRenderingComponentManager(),
                              frameRenderer);
    }
}

} // namespace hatcher
