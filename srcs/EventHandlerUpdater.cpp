#include "EventHandlerUpdater.hpp"

#include <SDL2/SDL_events.h>

#include "hatcher/GameApplication.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"

EventHandlerUpdater::EventHandlerUpdater(hatcher::GameApplication* application)
    : m_application(application)
{
}

void EventHandlerUpdater::Update(hatcher::ComponentManager* componentManager,
                                 hatcher::IFrameRenderer& frameRenderer)
{
    (void)componentManager;

    const float halfWidth = m_windowWidth / 2.f * m_pixelSize;
    const float halfHeight = m_windowHeight / 2.f * m_pixelSize;

    const float right = m_fixedPosition.x + halfWidth;
    const float left = m_fixedPosition.x - halfWidth;
    const float top = m_fixedPosition.y + halfHeight;
    const float bottom = m_fixedPosition.y - halfHeight;
    frameRenderer.SetProjectionMatrix(glm::ortho(left, right, top, bottom));

    const Uint8* keyState = SDL_GetKeyboardState(NULL);

    if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W])
        m_fixedPosition.y -= 0.1f;
    if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S])
        m_fixedPosition.y += 0.1f;
    if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D])
        m_fixedPosition.x += 0.1f;
    if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
        m_fixedPosition.x -= 0.1f;
    if (keyState[SDL_SCANCODE_ESCAPE])
        m_application->Stop();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            m_application->Stop();
        }
    }
}
