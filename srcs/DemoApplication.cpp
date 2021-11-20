#include "DemoApplication.hpp"

#include <SDL2/SDL.h>
#include <iostream>

#include "hatcher/Entity.hpp"
#include "hatcher/World.hpp"

#include "SquareDisplayUpdater.hpp"

DemoApplication::DemoApplication()
    : hatcher::GameApplication("hatcher - demo", 800, 600)
{
    std::shared_ptr<hatcher::World> world = CreateNewWorld("default");
    world->AddRenderingUpdater(new SquareDisplayUpdater());

    SetWatchedWorld(world);
}

DemoApplication::~DemoApplication() = default;

void DemoApplication::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            Stop();
        }
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                Stop();
            }
        }
    }
}
