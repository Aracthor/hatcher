#pragma once

#include <map>
#include <memory>
#include <vector>

#include <SDL2/SDL_events.h>

#include "hatcher/span.hpp"

namespace hatcher
{
class ComponentManager;
class IApplication;
class IEntityManager;
class IEventListener;
class IFrameRenderer;

class AbstractEventUpdater
{
public:
    virtual ~AbstractEventUpdater() = default;

    void PollEvents(IApplication* application, IEntityManager* entityManager,
                    ComponentManager* componentManager, ComponentManager* renderComponentManager,
                    const IFrameRenderer& frameRenderer);

    void Update(IEntityManager* entityManager, ComponentManager* componentManager,
                ComponentManager* renderComponentManager, IFrameRenderer& frameRenderer);

    void RegisterEventListener(const std::shared_ptr<IEventListener>& eventListener);

private:
    virtual void HandleEvents(const span<const SDL_Event>& events, IEntityManager* entityManager,
                              ComponentManager* componentManager,
                              ComponentManager* renderComponentManager,
                              IFrameRenderer& frameRenderer) = 0;

    std::vector<SDL_Event> m_queuedEvents;
    std::map<SDL_EventType, std::vector<std::shared_ptr<IEventListener>>> m_eventListeners;
};

} // namespace hatcher
