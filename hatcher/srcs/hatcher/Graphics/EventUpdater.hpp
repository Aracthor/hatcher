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
class WorldSettings;

class EventUpdater final
{
public:
    void ProcessApplicationEvents(span<const SDL_Event> events, WorldSettings& settings, IEntityManager* entityManager);
    void ProcessEventListeners(span<const SDL_Event> events, ICommandManager* commandManager,
                               const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                               const IFrameRenderer& frameRenderer);

    void RegisterListener(IEventListener* eventListener);

private:
    std::map<SDL_EventType, std::vector<IEventListener*>> m_eventListeners;

    std::vector<ubyte> m_saveState;
};

} // namespace hatcher
