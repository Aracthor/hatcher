#include "World.hpp"

#include <map>
#include <string>

#include "EntityManager.hpp"
#include "Updater.hpp"
#include "assert.hpp"

#include "Graphics/AbstractEventUpdater.hpp"
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

void World::UpdateRendering(IFrameRenderer& frameRenderer, const IRendering& rendering)
{
    if (m_eventUpdater)
    {
        m_eventUpdater->PollEvents();
        m_eventUpdater->Update(m_entityManager.get(), m_entityManager->GetComponentManager(),
                               frameRenderer);
    }

    for (std::unique_ptr<RenderUpdater>& renderUpdater : m_renderUpdaters)
    {
        renderUpdater->Update(m_entityManager->GetComponentManager(),
                              m_entityManager->GetRenderingComponentManager(), frameRenderer);
    }
}

} // namespace hatcher
