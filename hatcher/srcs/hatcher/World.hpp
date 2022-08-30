#pragma once

#include <memory>
#include <string>
#include <vector>

namespace hatcher
{
class EntityManager;
class EventUpdater;
class IApplication;
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
    void AddRenderUpdater(const char* name, const IRendering* rendering);
    void AddEventListener(const char* name);

    void Update();
    void UpdateRendering(IApplication* application, IFrameRenderer& frameRenderer,
                         const IRendering& rendering);

private:
    std::string m_name;

    std::unique_ptr<EntityManager> m_entityManager;

    std::vector<std::unique_ptr<Updater>> m_updaters;
    std::unique_ptr<EventUpdater> m_eventUpdater;
    std::vector<std::unique_ptr<RenderUpdater>> m_renderUpdaters;
};

} // namespace hatcher
