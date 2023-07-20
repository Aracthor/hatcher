#pragma once

#include <SDL2/SDL_events.h>

#include "hatcher/span.hpp"

namespace hatcher
{
class ComponentManager;
class IEntityManager;
class IFrameRenderer;

class IEventListener
{
public:
    virtual ~IEventListener() = default;

    virtual void GetEvent(const SDL_Event& event, IEntityManager* entityManager, ComponentManager* componentManager,
                          ComponentManager* renderComponentManager, const IFrameRenderer& frameRenderer) = 0;
    virtual span<const SDL_EventType> EventTypesToListen() const = 0;
};

} // namespace hatcher
