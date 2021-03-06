#pragma once

#include <vector>

#include <SDL2/SDL_events.h>

#include "hatcher/span.hpp"

namespace hatcher
{
class ComponentManager;
class IEntityManager;
class IFrameRenderer;

class AbstractEventUpdater
{
public:
    virtual ~AbstractEventUpdater() = default;

    void PollEvents();

    void Update(IEntityManager* entityManager, ComponentManager* componentManager,
                ComponentManager* renderComponentManager, IFrameRenderer& frameRenderer);

private:
    virtual void HandleEvents(const span<const SDL_Event>& events, IEntityManager* entityManager,
                              ComponentManager* componentManager,
                              ComponentManager* renderComponentManager,
                              IFrameRenderer& frameRenderer) = 0;

    std::vector<SDL_Event> m_queuedEvents;
};

} // namespace hatcher
