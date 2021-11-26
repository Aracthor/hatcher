#include "World.hpp"

#include "ComponentManager.hpp"
#include "Updater.hpp"
#include "assert.hpp"

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

void World::UpdateRendering(IFrameRenderer& frameRenderer)
{
    for (std::unique_ptr<RenderUpdater>& renderUpdater : m_renderUpdaters)
    {
        renderUpdater->Update(m_componentManager.get(), frameRenderer);
    }
}

} // namespace hatcher
