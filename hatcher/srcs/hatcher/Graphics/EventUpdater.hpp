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
class ICommandManager;
class IEventListener;
class IFrameRenderer;

class EventUpdater final
{
public:
    void ProcessEvents(span<const SDL_Event> events, IApplication* application, ICommandManager* commandManager,
                       const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                       const IFrameRenderer& frameRenderer);

    void RegisterListener(IEventListener* eventListener);

private:
    std::map<SDL_EventType, std::vector<IEventListener*>> m_eventListeners;
};

} // namespace hatcher
