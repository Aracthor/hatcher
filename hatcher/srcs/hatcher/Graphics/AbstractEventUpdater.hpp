#pragma once

#include <vector>

#include <SDL2/SDL_events.h>

#include "hatcher/span.hpp"

namespace hatcher
{
class ComponentManager;
class IEntityManager;
class IFrameRenderer;
class IRendering;

class AbstractEventUpdater
{
public:
    virtual ~AbstractEventUpdater() = default;

    void PollEvents();

    void Update(IEntityManager* entityManager, ComponentManager* componentManager,
                IFrameRenderer& frameRenderer, const IRendering& rendering);

private:
    virtual void HandleEvents(const span<const SDL_Event>& events, IEntityManager* entityManager,
                              ComponentManager* componentManager, IFrameRenderer& frameRenderer,
                              const IRendering& rendering) = 0;

    std::vector<SDL_Event> m_queuedEvents;
};

} // namespace hatcher
