#pragma once

#include <map>
#include <memory>
#include <vector>

#include <SDL2/SDL_events.h>

#include "IEventUpdater.hpp"

#include "hatcher/span.hpp"

namespace hatcher
{
class ComponentManager;
class IApplication;
class ICommandManager;
class IEventListener;
class IFrameRenderer;

class EventUpdater final : public IEventUpdater
{
public:
    void ProcessEvents(span<const SDL_Event> events, IApplication* application, ICommandManager* commandManager,
                       const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                       const IFrameRenderer& frameRenderer);

    void RegisterListener(const std::shared_ptr<IEventListener>& eventListener) override;

private:
    std::map<SDL_EventType, std::vector<std::shared_ptr<IEventListener>>> m_eventListeners;
};

} // namespace hatcher
