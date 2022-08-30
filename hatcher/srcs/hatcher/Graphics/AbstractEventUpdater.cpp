#include "AbstractEventUpdater.hpp"

#include "IEventListener.hpp"

namespace hatcher
{

void AbstractEventUpdater::PollEvents(IApplication* application, IEntityManager* entityManager,
                                      ComponentManager* componentManager,
                                      ComponentManager* renderComponentManager)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        m_queuedEvents.push_back(event);
        SDL_EventType eventType = static_cast<SDL_EventType>(event.type);
        if (m_eventListeners.find(eventType) != m_eventListeners.end())
        {
            for (auto& listener : m_eventListeners[eventType])
                listener->GetEvent(event, application, entityManager, componentManager,
                                   renderComponentManager);
        }
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

void AbstractEventUpdater::RegisterEventListener(
    const std::shared_ptr<IEventListener>& eventListener)
{
    const span<const SDL_EventType> eventTypesToListen = eventListener->EventTypesToListen();

    for (const SDL_EventType eventType : eventTypesToListen)
    {
        if (m_eventListeners.find(eventType) == m_eventListeners.end())
            m_eventListeners[eventType] = {};

        m_eventListeners[eventType].push_back(eventListener);
    }
}

} // namespace hatcher
