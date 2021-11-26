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

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            m_application->Stop();
        }
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                m_application->Stop();
                break;

            case SDLK_UP:
            case SDLK_w:
                m_fixedPosition.y -= 0.1f;
                break;

            case SDLK_DOWN:
            case SDLK_s:
                m_fixedPosition.y += 0.1f;
                break;

            case SDLK_RIGHT:
            case SDLK_r:
                m_fixedPosition.x += 0.1f;
                break;

            case SDLK_LEFT:
            case SDLK_l:
                m_fixedPosition.x -= 0.1f;
                break;

            default:
                break;
            }
        }
    }
}
