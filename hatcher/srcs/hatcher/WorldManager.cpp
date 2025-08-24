#include "WorldManager.hpp"

#include <map>
#include <vector>

#include "CommandManager.hpp"
#include "ComponentRegisterer.hpp"
#include "EntityDescriptorCatalog.hpp"
#include "EntityManager.hpp"
#include "Updater.hpp"
#include "World.hpp"

#include "Graphics/EventUpdater.hpp"
#include "Graphics/IEventListener.hpp"
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

using TSortedRenderUpdaterCreators = std::map<int, std::vector<const IRenderUpdaterCreator*>>;

TSortedRenderUpdaterCreators& RenderUpdaterCreators()
{
    static TSortedRenderUpdaterCreators renderUpdaterCreators;
    return renderUpdaterCreators;
}

std::vector<const IEventListenerCreator*>& EventListenerCreators()
{
    static std::vector<const IEventListenerCreator*> eventListenerCreators;
    return eventListenerCreators;
}

using TComponentCreators = std::vector<const IComponentTypeCreator*>[(int)EComponentList::COUNT];
using TWorldComponentCreators = std::vector<const IWorldComponentTypeCreator*>[(int)EComponentList::COUNT];

TComponentCreators& ComponentTypeCreators()
{
    static TComponentCreators creators;
    return creators;
}

TWorldComponentCreators& WorldComponentTypeCreators()
{
    static TWorldComponentCreators creators;
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

void RegisterRenderUpdater(const IRenderUpdaterCreator* creator, int order)
{
    RenderUpdaterCreators()[order].push_back(creator);
}

void RegisterEventListener(const IEventListenerCreator* creator)
{
    EventListenerCreators().push_back(creator);
}

void RegisterComponentTypeCreator(const IComponentTypeCreator* creator, EComponentList type)
{
    const int index = static_cast<int>(type);
    ComponentTypeCreators()[index].push_back(creator);
}

void RegisterWorldComponentTypeCreator(const IWorldComponentTypeCreator* creator, EComponentList type)
{
    const int index = static_cast<int>(type);
    WorldComponentTypeCreators()[index].push_back(creator);
}

void RegisterEntityDescriptor(EntityDescriptorID id, IEntityDescriptor* descriptor)
{
    GetEntityDescriptorCatalog()->AddEntityDescriptor(id, descriptor);
}

WorldManager::WorldManager()
{
    for (auto creator : UpdaterCreators())
    {
        m_updaters.emplace_back(creator->Create());
    }
}

WorldManager::~WorldManager() = default;

void WorldManager::CreateRenderUpdaters(const IRendering* rendering)
{
    m_eventUpdater = make_unique<EventUpdater>();
    for (auto it : RenderUpdaterCreators())
    {
        for (auto creator : it.second)
        {
            m_renderUpdaters.emplace_back(creator->Create(rendering));
        }
    }
    for (auto creator : EventListenerCreators())
    {
        m_eventUpdater->RegisterListener(creator->Create());
    }
}

unique_ptr<World> WorldManager::CreateWorld(int64_t seed, const std::optional<std::string>& commandSaveFile,
                                            const std::optional<std::string>& commandLoadFile) const
{
    unique_ptr<EntityManager> entityManager = make_unique<EntityManager>(GetEntityDescriptorCatalog());
    for (auto creator : ComponentTypeCreators()[(int)EComponentList::Gameplay])
    {
        creator->CreateComponentType(entityManager->GetComponentManager());
        creator->CreateComponentType(entityManager->GetTemporaryComponentManager());
    }
    for (auto creator : WorldComponentTypeCreators()[(int)EComponentList::Gameplay])
    {
        creator->CreateComponentType(entityManager->GetComponentManager(), seed);
        creator->CreateComponentType(entityManager->GetTemporaryComponentManager(), seed);
    }
    const bool hasRendering = m_eventUpdater;
    if (hasRendering)
    {
        for (auto creator : ComponentTypeCreators()[(int)EComponentList::Rendering])
        {
            creator->CreateComponentType(entityManager->GetRenderingComponentManager());
            creator->CreateComponentType(entityManager->GetTemporaryRenderingComponentManager());
        }
        for (auto creator : WorldComponentTypeCreators()[(int)EComponentList::Rendering])
        {
            creator->CreateComponentType(entityManager->GetRenderingComponentManager(), seed);
            creator->CreateComponentType(entityManager->GetTemporaryRenderingComponentManager(), seed);
        }
    }
    return make_unique<World>(std::move(entityManager), seed, commandSaveFile, commandLoadFile);
}

void WorldManager::Update(World* world)
{
    EntityManager* entityManager = world->GetEntityManager();
    for (unique_ptr<Updater>& updater : m_updaters)
    {
        updater->Update(entityManager, entityManager->GetComponentAccessor());
    }
    world->UpdateTickCommands();

    std::vector<Entity> entitiesAdded = {entityManager->EntitiesToAdd().begin(), entityManager->EntitiesToAdd().end()};
    for (const Entity entity : entityManager->EntitiesToDelete())
    {
        for (unique_ptr<Updater>& updater : m_updaters)
        {
            updater->OnDeletedEntity(entity, entityManager, entityManager->GetComponentAccessor());
        }
    }
    entityManager->UpdateNewAndDeletedEntities();
    for (const Entity entity : entitiesAdded)
    {
        for (unique_ptr<Updater>& updater : m_updaters)
        {
            updater->OnCreatedEntity(entity, entityManager, entityManager->GetComponentAccessor());
        }
        for (unique_ptr<RenderUpdater>& renderUpdater : m_renderUpdaters)
        {
            renderUpdater->OnCreateEntity(entity, entityManager->GetComponentAccessor(),
                                          entityManager->GetRenderingComponentAccessor());
        }
    }
    world->IncrementTick();
}

void WorldManager::UpdateFromEvents(span<const SDL_Event> events, IApplication* application, World* world,
                                    const IFrameRenderer& frameRenderer)
{
    EntityManager* entityManager = world->GetEntityManager();
    if (m_eventUpdater)
    {
        m_eventUpdater->ProcessApplicationEvents(events, entityManager);
        m_eventUpdater->ProcessEventListeners(events, application, world->GetCommandManager(),
                                              entityManager->GetComponentAccessor(),
                                              entityManager->GetRenderingComponentAccessor(), frameRenderer);
    }
}

void WorldManager::UpdateRendering(IApplication* application, World* world, IFrameRenderer& frameRenderer)
{
    EntityManager* entityManager = world->GetEntityManager();
    for (unique_ptr<RenderUpdater>& renderUpdater : m_renderUpdaters)
    {
        renderUpdater->Update(application, entityManager->GetComponentAccessor(),
                              entityManager->GetRenderingComponentAccessor(), frameRenderer);
    }
}

} // namespace hatcher
