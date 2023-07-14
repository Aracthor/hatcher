#include "Camera.hpp"
#include "HexagonalGrid.hpp"
#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "SelectableComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IEventUpdater.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"

namespace
{

using namespace hatcher;

class MoveOrderEventListener final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, IEntityManager* entityManager,
                  ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        HATCHER_ASSERT(event.type == SDL_MOUSEBUTTONDOWN);
        if (event.button.button == SDL_BUTTON_RIGHT)
        {
            const Camera* camera = renderComponentManager->ReadWorldComponent<Camera>();
            const glm::vec2 worldCoords2D =
                camera->MouseCoordsToWorldCoords(event.button.x, event.button.y, frameRenderer);
            const HexagonalGrid* hexaGrid = componentManager->ReadWorldComponent<HexagonalGrid>();
            const HexagonalGrid::TileCoord coords = hexaGrid->PositionToTileCoords(worldCoords2D);
            if (!hexaGrid->GetTileData(coords).walkable)
                return;

            const HexagonalGrid::TileCoord tileDestination =
                hexaGrid->PositionToTileCoords(worldCoords2D);
            auto movementComponents = componentManager->WriteComponents<Movement2DComponent>();
            auto selectableComponents =
                renderComponentManager->ReadComponents<SelectableComponent>();
            auto positionComponents = componentManager->ReadComponents<Position2DComponent>();

            HATCHER_ASSERT(componentManager->Count() == renderComponentManager->Count());
            for (int i = 0; i < componentManager->Count(); i++)
            {
                std::optional<Movement2DComponent>& movementComponent = movementComponents[i];
                const std::optional<SelectableComponent>& selectableComponent =
                    selectableComponents[i];
                const std::optional<Position2DComponent>& positionComponent = positionComponents[i];
                if (selectableComponent && selectableComponent->selected && movementComponent)
                {
                    HATCHER_ASSERT(positionComponent);
                    const HexagonalGrid::TileCoord tileStart =
                        hexaGrid->PositionToTileCoords(positionComponent->position);
                    std::vector<glm::vec2> path =
                        hexaGrid->GetPathIfPossible(tileStart, tileDestination);
                    if (!path.empty())
                    {
                        movementComponent->path = path;
                    }
                }
            }
        }
    }

    span<const SDL_EventType> EventTypesToListen() const override
    {
        static const SDL_EventType events[] = {
            SDL_MOUSEBUTTONDOWN,
        };
        return span<const SDL_EventType>(events, std::size(events));
    }
};

class MoveOrderRenderUpdater final : public RenderUpdater
{
public:
    MoveOrderRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
    {
        eventUpdater->RegisterListener(std::make_shared<MoveOrderEventListener>());
    }

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        // TODO show current path ?
    }
};

const int dummy = RegisterRenderUpdater<MoveOrderRenderUpdater>();

} // namespace
