#pragma once

#include <memory>
#include <string>
#include <vector>

namespace hatcher
{
class AbstractEventUpdater;
class Clock;
class EntityManager;
class IFrameRenderer;
class RenderUpdater;
class Updater;

class World final
{
public:
    World(const char* name);
    ~World();

    EntityManager* GetEntityManager() { return m_entityManager.get(); }

    void AddRenderUpdater(RenderUpdater* updater);
    void SetEventUpdater(AbstractEventUpdater* updater);

    void UpdateRendering(IFrameRenderer& frameRenderer, const Clock& clock);

private:
    std::string m_name;

    std::unique_ptr<EntityManager> m_entityManager;

    std::unique_ptr<AbstractEventUpdater> m_eventUpdater;
    std::vector<std::unique_ptr<RenderUpdater>> m_renderUpdaters;
};

} // namespace hatcher
