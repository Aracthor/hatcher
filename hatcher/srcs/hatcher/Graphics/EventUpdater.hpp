#pragma once

#include <map>
#include <memory>
#include <vector>

#include <SDL2/SDL_events.h>

namespace hatcher
{
class ComponentManager;
class IApplication;
class IEntityManager;
class IEventListener;
class IFrameRenderer;

class EventUpdater
{
public:
    void PollEvents(IApplication* application, IEntityManager* entityManager,
                    ComponentManager* componentManager, ComponentManager* renderComponentManager,
                    const IFrameRenderer& frameRenderer);

    void RegisterEventListener(const std::shared_ptr<IEventListener>& eventListener);

private:
    std::map<SDL_EventType, std::vector<std::shared_ptr<IEventListener>>> m_eventListeners;
};

} // namespace hatcher
