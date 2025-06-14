#pragma once

#include <SDL2/SDL_events.h>

namespace hatcher
{
class ComponentManager;
class ICommandManager;
class IFrameRenderer;

class IEventListener
{
public:
    virtual ~IEventListener() = default;

    virtual void GetEvent(const SDL_Event& event, ICommandManager* commandManager,
                          const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                          const IFrameRenderer& frameRenderer) = 0;
};

} // namespace hatcher
