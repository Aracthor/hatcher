#include "DemoApplication.hpp"

#include <iostream>

#include "hatcher/Entity.hpp"
#include "hatcher/World.hpp"
#include "hatcher/glm_pure.hpp"

#include "EventHandlerUpdater.hpp"
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
    world->AddRenderingUpdater(new EventHandlerUpdater(this));

    SetWatchedWorld(world);
}

DemoApplication::~DemoApplication() = default;
