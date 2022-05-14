#pragma once

#include <memory>
#include <string>
#include <vector>

namespace hatcher
{
class AbstractEventUpdater;
class EntityManager;
class IFrameRenderer;
class IRendering;
class RenderUpdater;
class Updater;

class World final
{
public:
    World(const char* name);
    ~World();

    EntityManager* GetEntityManager() { return m_entityManager.get(); }

    void AddUpdater(const char* name);
    void AddRenderUpdater(RenderUpdater* updater);
    void SetEventUpdater(AbstractEventUpdater* updater);

    void Update();
    void UpdateRendering(IFrameRenderer& frameRenderer, const IRendering& rendering);

private:
    std::string m_name;

    std::unique_ptr<EntityManager> m_entityManager;

    std::vector<std::unique_ptr<Updater>> m_updaters;
    std::unique_ptr<AbstractEventUpdater> m_eventUpdater;
    std::vector<std::unique_ptr<RenderUpdater>> m_renderUpdaters;
};

} // namespace hatcher
