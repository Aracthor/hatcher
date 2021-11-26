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

    const float width = 800.f;
    const float height = 600.f;
    const float right = width / 400.f;
    const float left = -width / 400.f;
    const float top = height / 400.f;
    const float bottom = -height / 400.f;
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
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                m_application->Stop();
            }
        }
    }
}
