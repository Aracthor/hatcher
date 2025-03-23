#pragma once

#include <map>
#include <vector>

#include <SDL2/SDL_events.h>

#include "hatcher/basic_types.hpp"
#include "hatcher/span.hpp"

namespace hatcher
{
class ComponentManager;
class ICommandManager;
class IEntityManager;
class IEventListener;
class IFrameRenderer;

class EventUpdater final
{
public:
    void ProcessEvents(span<const SDL_Event> events, IEntityManager* entityManager, ICommandManager* commandManager,
                       const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                       const IFrameRenderer& frameRenderer);

    void RegisterListener(IEventListener* eventListener);

private:
    std::map<SDL_EventType, std::vector<IEventListener*>> m_eventListeners;

    std::vector<ubyte> m_saveState;
};

} // namespace hatcher
