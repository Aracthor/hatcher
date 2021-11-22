#include "EventHandlerUpdater.hpp"

#include <SDL2/SDL_events.h>

#include "hatcher/GameApplication.hpp"

EventHandlerUpdater::EventHandlerUpdater(hatcher::GameApplication* application)
    : m_application(application)
{
}

void EventHandlerUpdater::Update(hatcher::ComponentManager* componentManager)
{
    (void)componentManager;
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
