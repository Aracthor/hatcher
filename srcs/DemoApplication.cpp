#include "DemoApplication.hpp"

#include <iostream>

#include "hatcher/ComponentManager.hpp"
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
    hatcher::ComponentManager* componentManager = world->GetComponentManager();

    componentManager->AddComponentType<Position2DComponent>();
    hatcher::Entity entity = componentManager->CreateNewEntity();
    Position2DComponent position2D{glm::vec2(0.5f, 0.0f)};
    componentManager->AttachComponent<Position2DComponent>(entity, position2D);

    world->AddRenderingUpdater(new SquareDisplayUpdater(GetMeshBuilder()));
    world->AddRenderingUpdater(new EventHandlerUpdater(this));

    SetWatchedWorld(world);
}

DemoApplication::~DemoApplication() = default;
