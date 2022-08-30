#include "World.hpp"

#include <iostream>
#include <map>
#include <string>

#include "EntityManager.hpp"
#include "Updater.hpp"
#include "assert.hpp"

#include "Graphics/AbstractEventUpdater.hpp"
#include "Graphics/IEventListener.hpp"
#include "Graphics/RenderUpdater.hpp"

namespace hatcher
{

namespace
{
std::map<std::string, CreateUpdaterFunction*>& UpdaterCreators()
{
    static std::map<std::string, CreateUpdaterFunction*> updaterCreators;
    return updaterCreators;
}

std::map<std::string, CreateRenderUpdaterFunction*>& RenderUpdaterCreators()
{
    static std::map<std::string, CreateRenderUpdaterFunction*> updaterCreators;
    return updaterCreators;
}

std::map<std::string, CreateEventListenerFunction*>& EventListenerCreators()
{
    static std::map<std::string, CreateEventListenerFunction*> eventListenerCreators;
    return eventListenerCreators;
}
} // namespace

int RegisterUpdater(const char* name, CreateUpdaterFunction* createFunction)
{
    auto& updaterCreators = UpdaterCreators();
    if (updaterCreators.find(name) != updaterCreators.end())
    {
        std::cerr << "Updater registered twice: " << name << std::endl;
        std::abort();
    }
    updaterCreators[name] = createFunction;
    return 0;
}

int RegisterRenderUpdater(const char* name, CreateRenderUpdaterFunction* createFunction)
{
    auto& renderUpdaterCreators = RenderUpdaterCreators();
    if (renderUpdaterCreators.find(name) != renderUpdaterCreators.end())
    {
        std::cerr << "RenderUpdater registered twice: " << name << std::endl;
        std::abort();
    }
    renderUpdaterCreators[name] = createFunction;
    return 0;
}

int RegisterEventListener(const char* name, CreateEventListenerFunction* createFunction)
{
    auto& eventListenerCreators = EventListenerCreators();
    if (eventListenerCreators.find(name) != eventListenerCreators.end())
    {
        std::cerr << "EventListener registered twice: " << name << std::endl;
        std::abort();
    }
    eventListenerCreators[name] = createFunction;
    return 0;
}

World::World(const char* name)
    : m_name(name)
{
    m_entityManager.reset(new EntityManager());
    for (auto entry : UpdaterCreators())
    {
        m_updaters.emplace_back(entry.second());
    }
}

World::~World() = default;

void World::AddUpdater(const char* name)
{
    auto& updaterCreators = UpdaterCreators();
    if (updaterCreators.find(name) == updaterCreators.end())
    {
        std::cerr << "Unkown updater: " << name << std::endl;
        std::abort();
    }
    m_updaters.emplace_back(updaterCreators[name]());
}

void World::AddRenderUpdater(const char* name, const IRendering* rendering)
{
    auto& updaterCreators = RenderUpdaterCreators();
    if (updaterCreators.find(name) == updaterCreators.end())
    {
        std::cerr << "Unkown render updater: " << name << std::endl;
        std::abort();
    }
    m_renderUpdaters.emplace_back(RenderUpdaterCreators()[name](rendering));
}

void World::AddEventListener(const char* name)
{
    auto& eventListenerCreators = EventListenerCreators();
    if (eventListenerCreators.find(name) == eventListenerCreators.end())
    {
        std::cerr << "Unkown event listener: " << name << std::endl;
        std::abort();
    }
    std::shared_ptr<IEventListener> eventListener(eventListenerCreators[name]());
    m_eventUpdater->RegisterEventListener(eventListener);
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

void World::UpdateRendering(IApplication* application, IFrameRenderer& frameRenderer,
                            const IRendering& rendering)
{
    if (m_eventUpdater)
    {
        m_eventUpdater->PollEvents(application, m_entityManager.get(),
                                   m_entityManager->GetComponentManager(),
                                   m_entityManager->GetRenderingComponentManager(), frameRenderer);
        m_eventUpdater->Update(m_entityManager.get(), m_entityManager->GetComponentManager(),
                               m_entityManager->GetRenderingComponentManager(), frameRenderer);
    }

    for (std::unique_ptr<RenderUpdater>& renderUpdater : m_renderUpdaters)
    {
        renderUpdater->Update(m_entityManager->GetComponentManager(),
                              m_entityManager->GetRenderingComponentManager(), frameRenderer);
    }
}

} // namespace hatcher
