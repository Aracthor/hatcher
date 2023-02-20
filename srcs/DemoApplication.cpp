#include "DemoApplication.hpp"

#include <iostream>

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Entity.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/World.hpp"
#include "hatcher/glm_pure.hpp"

#include "Camera.hpp"
#include "HexagonalGrid.hpp"
#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "Selectable2DComponent.hpp"

using namespace hatcher;

namespace
{
void InitHexaGrid(HexagonalGrid* grid)
{
    for (int q = -5; q <= 5; q++)
    {
        for (int r = -5; r <= 5; r++)
        {
            grid->SetTileWalkable(HexagonalGrid::TileCoord(q, r), true);
        }
    }
    grid->SetTileWalkable(HexagonalGrid::TileCoord(0, 0), false);
}
} // namespace

DemoApplication::DemoApplication()
    : GameApplication()
{
    std::shared_ptr<World> world = CreateNewWorld("default");
    EntityManager* entityManager = world->GetEntityManager();
    ComponentManager* componentManager = entityManager->GetComponentManager();
    ComponentManager* renderComponentManager = entityManager->GetRenderingComponentManager();

    componentManager->AddComponentType<Position2DComponent>();
    componentManager->AddComponentType<Movement2DComponent>();
    componentManager->AddWorldComponent<HexagonalGrid>();
    InitHexaGrid(componentManager->WriteWorldComponent<HexagonalGrid>());

    renderComponentManager->AddComponentType<Selectable2DComponent>();
    renderComponentManager->AddWorldComponent<Camera>();

    world->AddUpdater("MovingEntities");

    StartRendering("hatcher - demo", 800, 600);

    world->AddRenderUpdater("Camera", GetRendering());
    world->AddRenderUpdater("CubeDisplay", GetRendering());
    world->AddRenderUpdater("DebugGrid", GetRendering());
    world->AddRenderUpdater("DemoImgui", GetRendering());
    world->AddRenderUpdater("EntityCreator", GetRendering());
    world->AddRenderUpdater("HexaGrid", GetRendering());
    world->AddRenderUpdater("MoveOrder", GetRendering());
    world->AddRenderUpdater("MovingEntities", GetRendering());
    world->AddRenderUpdater("SaveLoader", GetRendering());
    world->AddRenderUpdater("Selected", GetRendering());
    world->AddRenderUpdater("SelectionRectangle", GetRendering());
}

DemoApplication::~DemoApplication() = default;
