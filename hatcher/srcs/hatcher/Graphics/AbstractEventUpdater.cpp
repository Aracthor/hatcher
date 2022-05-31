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
                                  ComponentManager* renderComponentManager,
                                  IFrameRenderer& frameRenderer)
{
    span<const SDL_Event> events(m_queuedEvents);
    HandleEvents(events, entityManager, componentManager, renderComponentManager, frameRenderer);
    m_queuedEvents.clear();
}

} // namespace hatcher
