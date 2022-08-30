#include "DemoApplication.hpp"

#include <iostream>

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Entity.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/World.hpp"
#include "hatcher/glm_pure.hpp"

#include "Camera.hpp"
#include "GridDisplay.hpp"
#include "Movement2DComponent.hpp"
#include "Obstacle2DComponent.hpp"
#include "ObstacleMeshComponent.hpp"
#include "Position2DComponent.hpp"
#include "Selectable2DComponent.hpp"
#include "SelectionRectangle.hpp"

using namespace hatcher;

DemoApplication::DemoApplication()
    : GameApplication()
{
    std::shared_ptr<World> world = CreateNewWorld("default");
    EntityManager* entityManager = world->GetEntityManager();
    ComponentManager* componentManager = entityManager->GetComponentManager();
    ComponentManager* renderComponentManager = entityManager->GetRenderingComponentManager();

    componentManager->AddComponentType<Position2DComponent>();
    componentManager->AddComponentType<Movement2DComponent>();
    componentManager->AddComponentType<Obstacle2DComponent>();

    renderComponentManager->AddComponentType<ObstacleMeshComponent>();
    renderComponentManager->AddComponentType<Selectable2DComponent>();
    renderComponentManager->AddWorldComponent<Camera>();
    renderComponentManager->AddWorldComponent<GridDisplay>();
    renderComponentManager->AddWorldComponent<SelectionRectangle>();

    world->AddUpdater("MovingEntities");

    Entity obstacleEntity = entityManager->CreateNewEntity();
    Obstacle2DComponent obstacle2D{{
        glm::vec2(2.5f, 3.0f),
        glm::vec2(1.5f, 3.0f),
        glm::vec2(1.5f, 1.0f),
        glm::vec2(3.5f, 1.0f),
    }};
    componentManager->AttachComponent<Obstacle2DComponent>(obstacleEntity, obstacle2D);

    StartRendering("hatcher - demo", 800, 600);

    world->AddRenderUpdater("Camera", GetRendering());
    world->AddRenderUpdater("CubeDisplay", GetRendering());
    world->AddRenderUpdater("DebugGrid", GetRendering());
    world->AddRenderUpdater("MovingEntities", GetRendering());
    world->AddRenderUpdater("Obstacle", GetRendering());
    world->AddRenderUpdater("Selected", GetRendering());
    world->AddRenderUpdater("SelectionRectangle", GetRendering());
    world->AddEventListener("Camera");
    world->AddEventListener("DebugGrid");
    world->AddEventListener("EntityCreator");
    world->AddEventListener("MoveOrder");
    world->AddEventListener("Quit");
    world->AddEventListener("SelectionRectangle");
}

DemoApplication::~DemoApplication() = default;
