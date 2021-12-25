#pragma once

#include <span>
#include <vector>

#include <SDL2/SDL_events.h>

namespace hatcher
{
class Clock;
class ComponentManager;
class IEntityManager;
class IFrameRenderer;

class AbstractEventUpdater
{
public:
    virtual ~AbstractEventUpdater() = default;

    void PollEvents();

    void Update(IEntityManager* entityManager, ComponentManager* componentManager,
                const Clock& clock, IFrameRenderer& frameRenderer);

private:
    virtual void HandleEvents(std::span<const SDL_Event> events, IEntityManager* entityManager,
                              ComponentManager* componentManager, const Clock& clock,
                              IFrameRenderer& frameRenderer) = 0;

    std::vector<SDL_Event> m_queuedEvents;
};

} // namespace hatcher
