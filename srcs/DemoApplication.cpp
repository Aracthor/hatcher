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

DemoApplication::DemoApplication(const char* commandName)
    : GameApplication(commandName)
{
    World* world = CreateNewWorld("default");
    EntityManager* entityManager = world->GetEntityManager();
    ComponentManager* componentManager = entityManager->GetComponentManager();
    ComponentManager* renderComponentManager = entityManager->GetRenderingComponentManager();

    componentManager->AddWorldComponent<HexagonalGrid>();
    InitHexaGrid(componentManager->WriteWorldComponent<HexagonalGrid>());

    renderComponentManager->AddWorldComponent<Camera>();

    StartRendering("hatcher - demo", 800, 600);
}

DemoApplication::~DemoApplication() = default;
