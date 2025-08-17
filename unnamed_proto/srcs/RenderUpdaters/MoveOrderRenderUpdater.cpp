#include "Components/Movement2DComponent.hpp"
#include "Components/Position2DComponent.hpp"
#include "RenderComponents/SelectableComponent.hpp"
#include "WorldComponents/Camera.hpp"
#include "WorldComponents/SquareGrid.hpp"

#include "hatcher/CommandRegisterer.hpp"
#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/ICommand.hpp"
#include "hatcher/ICommandManager.hpp"
#include "hatcher/IEntityManager.hpp"

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

    void Execute(IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        componentAccessor->WriteComponents<Movement2DComponent>()[m_entity]->path = m_path;
    }

private:
    const Entity m_entity;
    const std::vector<glm::vec2> m_path;

    COMMAND_HEADER(MoveOrderCommand)
};
REGISTER_COMMAND(MoveOrderCommand)

class MoveOrderRenderUpdater final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentAccessor* componentAccessor, ComponentAccessor* renderComponentAccessor,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT)
        {
            const Camera* camera = renderComponentAccessor->ReadWorldComponent<Camera>();
            const glm::vec2 worldCoords2D =
                camera->MouseCoordsToWorldCoords(event.button.x, event.button.y, frameRenderer);
            const SquareGrid* grid = componentAccessor->ReadWorldComponent<SquareGrid>();
            if (!grid->GetTileData(worldCoords2D).walkable)
                return;

            auto movementComponents = componentAccessor->ReadComponents<Movement2DComponent>();
            auto selectableComponents = renderComponentAccessor->ReadComponents<SelectableComponent>();
            auto positionComponents = componentAccessor->ReadComponents<Position2DComponent>();

            HATCHER_ASSERT(componentAccessor->Count() == renderComponentAccessor->Count());
            for (int i = 0; i < componentAccessor->Count(); i++)
            {
                const std::optional<Movement2DComponent>& movementComponent = movementComponents[i];
                const std::optional<SelectableComponent>& selectableComponent = selectableComponents[i];
                const std::optional<Position2DComponent>& positionComponent = positionComponents[i];
                if (selectableComponent && selectableComponent->selected && movementComponent)
                {
                    HATCHER_ASSERT(positionComponent);
                    std::vector<glm::vec2> path = grid->GetPathIfPossible(positionComponent->position, worldCoords2D);
                    if (!path.empty())
                    {
                        commandManager->AddCommand(new MoveOrderCommand(Entity(i), path));
                    }
                }
            }
        }
    }
};

EventListenerRegisterer<MoveOrderRenderUpdater> registerer;

} // namespace
