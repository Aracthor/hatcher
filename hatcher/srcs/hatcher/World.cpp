#include "World.hpp"

#include <vector>

#include "CommandManager.hpp"
#include "CommandSaveLoad.hpp"
#include "ComponentRegisterer.hpp"
#include "EntityDescriptorCatalog.hpp"
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

using TComponentCreators = std::vector<const IComponentTypeCreator*>[(int)EComponentList::COUNT];

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

EntityDescriptorCatalog* GetEntityDescriptorCatalog()
{
    static EntityDescriptorCatalog catalog;
    return &catalog;
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

void RegisterComponentTypeCreator(const IComponentTypeCreator* creator, EComponentList type)
{
    const int index = static_cast<int>(type);
    ComponentTypeCreators()[index].push_back(creator);
}

void RegisterWorldComponentTypeCreator(const IComponentTypeCreator* creator, EComponentList type)
{
    const int index = static_cast<int>(type);
    WorldComponentTypeCreators()[index].push_back(creator);
}

void RegisterEntityDescriptor(EntityDescriptorID id, IEntityDescriptor* descriptor)
{
    GetEntityDescriptorCatalog()->AddEntityDescriptor(id, descriptor);
}

World::World(int64_t seed, const std::optional<std::string>& commandSaveFile,
             const std::optional<std::string>& commandLoadFile)
    : m_settings({seed})
{
    m_entityManager = make_unique<EntityManager>(GetEntityDescriptorCatalog());
    for (auto creator : ComponentTypeCreators()[(int)EComponentList::Gameplay])
    {
        creator->CreateComponentType(m_entityManager->GetComponentManager());
        creator->CreateComponentType(m_entityManager->GetTemporaryComponentManager());
    }
    for (auto creator : WorldComponentTypeCreators()[(int)EComponentList::Gameplay])
    {
        creator->CreateComponentType(m_entityManager->GetComponentManager());
    }
    for (auto creator : UpdaterCreators())
    {
        m_updaters.emplace_back(creator->Create());
    }
    m_commandManager = make_unique<CommandManager>();
    if (commandSaveFile)
        m_commandSaver.emplace(new CommandSaver(*commandSaveFile));
    if (commandLoadFile)
        m_commandLoader.emplace(new CommandLoader(*commandLoadFile));
}

World::~World() = default;

void World::CreateRenderUpdaters(const IRendering* rendering)
{
    m_eventUpdater = make_unique<EventUpdater>();
    for (auto creator : ComponentTypeCreators()[(int)EComponentList::Rendering])
    {
        creator->CreateComponentType(m_entityManager->GetRenderingComponentManager());
        creator->CreateComponentType(m_entityManager->GetTemporaryRenderingComponentManager());
    }
    for (auto creator : WorldComponentTypeCreators()[(int)EComponentList::Rendering])
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
        updater->Update(m_settings, m_entityManager.get(), m_entityManager->GetComponentManager());
    }
    if (m_commandSaver)
    {
        for (const unique_ptr<ICommand>& command : m_commandManager->CurrentCommands())
        {
            (*m_commandSaver)->Save(m_tick, command.get());
        }
    }
    if (m_commandLoader)
    {
        for (ICommand* command : (*m_commandLoader)->CommandsForTick(m_tick))
        {
            m_commandManager->AddCommand(command);
        }
    }
    m_commandManager->ExecuteCommands(m_entityManager.get(), m_entityManager->GetComponentManager());
    for (const Entity entity : m_entityManager->EntitiesToDelete())
    {
        for (unique_ptr<Updater>& updater : m_updaters)
        {
            updater->OnDeletedEntity(entity, m_settings, m_entityManager.get(), m_entityManager->GetComponentManager());
        }
    }
    m_entityManager->UpdateNewAndDeletedEntities();
    m_tick++;
}

void World::UpdateFromEvents(span<const SDL_Event> events, IApplication* application,
                             const IFrameRenderer& frameRenderer)
{
    if (m_eventUpdater)
    {
        m_eventUpdater->ProcessApplicationEvents(events, m_settings, m_entityManager.get());
        m_eventUpdater->ProcessEventListeners(events, application, m_commandManager.get(),
                                              m_entityManager->GetComponentManager(),
                                              m_entityManager->GetRenderingComponentManager(), frameRenderer);
    }
}

void World::UpdateRendering(IApplication* application, IFrameRenderer& frameRenderer)
{
    for (unique_ptr<RenderUpdater>& renderUpdater : m_renderUpdaters)
    {
        renderUpdater->Update(application, m_entityManager->GetComponentManager(),
                              m_entityManager->GetRenderingComponentManager(), frameRenderer);
    }
}

} // namespace hatcher
