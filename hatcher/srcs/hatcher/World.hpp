#pragma once

#include <vector>

#include "span.hpp"
#include "unique_ptr.hpp"

union SDL_Event;
namespace hatcher
{
class CommandManager;
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
    World();
    ~World();

    void CreateRenderUpdaters(const IRendering* rendering);

    void Update();
    void UpdateFromEvents(span<const SDL_Event> events, IApplication* application, IFrameRenderer& frameRenderer);
    void UpdateRendering(IFrameRenderer& frameRenderer);

private:
    unique_ptr<EntityManager> m_entityManager;

    std::vector<unique_ptr<Updater>> m_updaters;
    unique_ptr<CommandManager> m_commandManager;

    unique_ptr<EventUpdater> m_eventUpdater;
    std::vector<unique_ptr<RenderUpdater>> m_renderUpdaters;
};

} // namespace hatcher
