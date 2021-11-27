#include "World.hpp"

#include "ComponentManager.hpp"
#include "Updater.hpp"
#include "assert.hpp"

#include "Graphics/IEventUpdater.hpp"
#include "Graphics/RenderUpdater.hpp"

#include <vector>

namespace hatcher
{

World::World(const char* name)
    : m_name(name)
{
    m_componentManager.reset(new ComponentManager());
}

World::~World() = default;

void World::AddRenderUpdater(RenderUpdater* updater)
{
    HATCHER_ASSERT(updater != nullptr);
    m_renderUpdaters.emplace_back(updater);
}

void World::SetEventUpdater(IEventUpdater* updater)
{
    HATCHER_ASSERT(updater != nullptr);
    m_eventUpdater.reset(updater);
}

void World::UpdateRendering(IFrameRenderer& frameRenderer, const Clock& clock)
{
    if (m_eventUpdater)
        m_eventUpdater->Update(m_componentManager.get(), clock, frameRenderer);

    for (std::unique_ptr<RenderUpdater>& renderUpdater : m_renderUpdaters)
    {
        renderUpdater->Update(m_componentManager.get(), clock, frameRenderer);
    }
}

} // namespace hatcher
