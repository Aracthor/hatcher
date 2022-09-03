#include "EventUpdater.hpp"

#include "IEventListener.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/IApplication.hpp"

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
        if (eventType == SDL_QUIT)
            application->Stop();
        else if (m_eventListeners.find(eventType) != m_eventListeners.end())
        {
            // We invert Y axis for mouse events because there seems to be an inversion between
            // OpenGL and SDL2.
            if (eventType == SDL_MOUSEBUTTONDOWN || eventType == SDL_MOUSEBUTTONUP)
                event.button.y = frameRenderer.Resolution().y - event.button.y;
            if (eventType == SDL_MOUSEMOTION)
                event.motion.y = frameRenderer.Resolution().y - event.motion.y;

            for (auto& listener : m_eventListeners[eventType])
                listener->GetEvent(event, entityManager, componentManager, renderComponentManager,
                                   frameRenderer);
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
