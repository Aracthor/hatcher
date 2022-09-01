#include "EventUpdater.hpp"

#include "IEventListener.hpp"

#include "backends/imgui_impl_sdl.h"

namespace hatcher
{

void EventUpdater::PollEvents(IApplication* application, IEntityManager* entityManager,
                              ComponentManager* componentManager,
                              ComponentManager* renderComponentManager,
                              const IFrameRenderer& frameRenderer)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // TODO a shame this ImGui function is not in ImGuiIntegration...
        ImGui_ImplSDL2_ProcessEvent(&event);
        SDL_EventType eventType = static_cast<SDL_EventType>(event.type);
        if (m_eventListeners.find(eventType) != m_eventListeners.end())
        {
            for (auto& listener : m_eventListeners[eventType])
                listener->GetEvent(event, application, entityManager, componentManager,
                                   renderComponentManager, frameRenderer);
        }
    }
}

void EventUpdater::RegisterEventListener(const std::shared_ptr<IEventListener>& eventListener)
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
