#include "DemoApplication.hpp"

#include <SDL2/SDL.h>
#include <iostream>

#include "hatcher/Entity.hpp"
#include "hatcher/World.hpp"
#include "hatcher/glm_pure.hpp"

#include "Position2DComponent.hpp"
#include "SquareDisplayUpdater.hpp"

DemoApplication::DemoApplication()
    : hatcher::GameApplication("hatcher - demo", 800, 600)
{
    std::shared_ptr<hatcher::World> world = CreateNewWorld("default");

    world->AddComponentType<Position2DComponent>();
    hatcher::Entity entity = world->CreateNewEntity();
    Position2DComponent position2D{glm::vec2(0.5f, 0.0f)};
    world->AttachComponent<Position2DComponent>(entity, position2D);
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
