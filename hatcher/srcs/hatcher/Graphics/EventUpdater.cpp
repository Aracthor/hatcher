#include "EventUpdater.hpp"

#include "IEventListener.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/IApplication.hpp"

namespace hatcher
{

void EventUpdater::ProcessEvents(span<const SDL_Event> events, IApplication* application,
                                 ICommandManager* commandManager, const ComponentManager* componentManager,
                                 ComponentManager* renderComponentManager, const IFrameRenderer& frameRenderer)
{
    for (const SDL_Event& event : events)
    {
        SDL_EventType eventType = static_cast<SDL_EventType>(event.type);
        if (eventType == SDL_QUIT)
            application->Stop();
        else if (m_eventListeners.find(eventType) != m_eventListeners.end())
        {
            for (auto& listener : m_eventListeners[eventType])
                listener->GetEvent(event, commandManager, componentManager, renderComponentManager, frameRenderer);
        }
    }
}

void EventUpdater::RegisterListener(const std::shared_ptr<IEventListener>& eventListener)
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
