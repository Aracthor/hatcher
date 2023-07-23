#include "Camera.hpp"
#include "HexagonalGrid.hpp"
#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "SelectableComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IEventUpdater.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/ICommand.hpp"
#include "hatcher/ICommandManager.hpp"

namespace
{

using namespace hatcher;

class MoveOrderCommand final : public ICommand
{
public:
    MoveOrderCommand(Entity entity, const std::vector<glm::vec2>& path)
        : m_entity(entity)
        , m_path(path)
    {
    }

    void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                 ComponentManager* renderingComponentManager) override
    {
        componentManager->WriteComponents<Movement2DComponent>()[m_entity]->path = m_path;
    }

private:
    const Entity m_entity;
    const std::vector<glm::vec2> m_path;
};

class MoveOrderEventListener final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, ICommandManager* commandManager, const ComponentManager* componentManager,
                  ComponentManager* renderComponentManager, const IFrameRenderer& frameRenderer) override
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

            const HexagonalGrid::TileCoord tileDestination = hexaGrid->PositionToTileCoords(worldCoords2D);
            auto movementComponents = componentManager->ReadComponents<Movement2DComponent>();
            auto selectableComponents = renderComponentManager->ReadComponents<SelectableComponent>();
            auto positionComponents = componentManager->ReadComponents<Position2DComponent>();

            HATCHER_ASSERT(componentManager->Count() == renderComponentManager->Count());
            for (int i = 0; i < componentManager->Count(); i++)
            {
                const std::optional<Movement2DComponent>& movementComponent = movementComponents[i];
                const std::optional<SelectableComponent>& selectableComponent = selectableComponents[i];
                const std::optional<Position2DComponent>& positionComponent = positionComponents[i];
                if (selectableComponent && selectableComponent->selected && movementComponent)
                {
                    HATCHER_ASSERT(positionComponent);
                    const HexagonalGrid::TileCoord tileStart =
                        hexaGrid->PositionToTileCoords(positionComponent->position);
                    std::vector<glm::vec2> path = hexaGrid->GetPathIfPossible(tileStart, tileDestination);
                    if (!path.empty())
                    {
                        commandManager->AddCommand(new MoveOrderCommand(Entity(i), path));
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

RenderUpdaterRegisterer<MoveOrderRenderUpdater> registerer;

} // namespace
