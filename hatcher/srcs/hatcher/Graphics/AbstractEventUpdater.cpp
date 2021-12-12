#include "AbstractEventUpdater.hpp"

namespace hatcher
{

void AbstractEventUpdater::PollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        m_queuedEvents.push_back(event);
    }
}

void AbstractEventUpdater::Update(ComponentManager* componentManager, const Clock& clock,
                                  IFrameRenderer& frameRenderer)
{
    HandleEvents(std::span(m_queuedEvents), componentManager, clock, frameRenderer);
    m_queuedEvents.clear();
}

} // namespace hatcher
