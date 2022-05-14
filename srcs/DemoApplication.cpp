#include "DemoApplication.hpp"

#include <iostream>

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Entity.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/World.hpp"
#include "hatcher/glm_pure.hpp"

#include "CubeDisplayUpdater.hpp"
#include "EventHandlerUpdater.hpp"
#include "Movement2DComponent.hpp"
#include "MovingEntitiesRenderUpdater.hpp"
#include "Obstacle2DComponent.hpp"
#include "ObstacleMeshComponent.hpp"
#include "ObstacleRenderUpdater.hpp"
#include "Position2DComponent.hpp"
#include "Selectable2DComponent.hpp"
#include "SelectedRenderUpdater.hpp"

DemoApplication::DemoApplication()
    : hatcher::GameApplication()
{
    std::shared_ptr<hatcher::World> world = CreateNewWorld("default");
    hatcher::EntityManager* entityManager = world->GetEntityManager();
    hatcher::ComponentManager* componentManager = entityManager->GetComponentManager();
    hatcher::ComponentManager* renderComponentManager =
        entityManager->GetRenderingComponentManager();

    componentManager->AddComponentType<Position2DComponent>();
    componentManager->AddComponentType<Movement2DComponent>();
    componentManager->AddComponentType<Obstacle2DComponent>();
    componentManager->AddComponentType<Selectable2DComponent>();

    renderComponentManager->AddComponentType<ObstacleMeshComponent>();

    world->AddUpdater("MovingEntities");

    hatcher::Entity obstacleEntity = entityManager->CreateNewEntity();
    Obstacle2DComponent obstacle2D{{
        glm::vec2(2.5f, 3.0f),
        glm::vec2(1.5f, 3.0f),
        glm::vec2(1.5f, 1.0f),
        glm::vec2(3.5f, 1.0f),
    }};
    componentManager->AttachComponent<Obstacle2DComponent>(obstacleEntity, obstacle2D);

    StartRendering("hatcher - demo", 800, 600);

    world->AddRenderUpdater(new CubeDisplayUpdater(GetRendering()));
    world->AddRenderUpdater(new MovingEntitiesRenderUpdater(GetRendering()));
    world->AddRenderUpdater(new ObstacleRenderUpdater(GetRendering()));
    world->AddRenderUpdater(new SelectedRenderUpdater(GetRendering()));
    world->SetEventUpdater(new EventHandlerUpdater(this, GetRendering()));
}

DemoApplication::~DemoApplication() = default;
