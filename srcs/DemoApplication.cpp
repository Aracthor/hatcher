#include "DemoApplication.hpp"

#include <iostream>

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Entity.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/World.hpp"
#include "hatcher/glm_pure.hpp"

#include "EventHandlerUpdater.hpp"
#include "Movement2DComponent.hpp"
#include "MovingEntitiesRenderUpdater.hpp"
#include "Position2DComponent.hpp"
#include "Selectable2DComponent.hpp"
#include "SelectedRenderUpdater.hpp"
#include "SquareDisplayUpdater.hpp"

DemoApplication::DemoApplication()
    : hatcher::GameApplication()
{
    std::shared_ptr<hatcher::World> world = CreateNewWorld("default");
    hatcher::ComponentManager* componentManager = world->GetComponentManager();

    componentManager->AddComponentType<Position2DComponent>();
    componentManager->AddComponentType<Movement2DComponent>();
    componentManager->AddComponentType<Selectable2DComponent>();
    hatcher::Entity entity = componentManager->CreateNewEntity();
    Position2DComponent position2D{glm::vec2(0.5f, 0.0f)};
    componentManager->AttachComponent<Position2DComponent>(entity, position2D);

    StartRendering("hatcher - demo", 800, 600);

    world->AddRenderUpdater(new SquareDisplayUpdater(GetRendering()->GetMeshBuilder()));
    world->AddRenderUpdater(new MovingEntitiesRenderUpdater(GetRendering()->GetMeshBuilder()));
    world->AddRenderUpdater(new SelectedRenderUpdater(GetRendering()->GetMeshBuilder()));
    world->SetEventUpdater(new EventHandlerUpdater(this, GetRendering()->GetMeshBuilder()));

    SetWatchedWorld(world);
}

DemoApplication::~DemoApplication() = default;
