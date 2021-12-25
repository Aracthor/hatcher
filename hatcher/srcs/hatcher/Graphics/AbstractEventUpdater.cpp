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

void AbstractEventUpdater::Update(IEntityManager* entityManager, ComponentManager* componentManager,
                                  const Clock& clock, IFrameRenderer& frameRenderer)
{
    HandleEvents(std::span(m_queuedEvents), entityManager, componentManager, clock, frameRenderer);
    m_queuedEvents.clear();
}

} // namespace hatcher
