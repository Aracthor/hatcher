#include "Camera.hpp"
#include "Movement2DComponent.hpp"
#include "Pathfinding.hpp"
#include "Position2DComponent.hpp"
#include "Selectable2DComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"

namespace
{

using namespace hatcher;

class MoveOrderEventListener final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, IApplication* gameApplication,
                  IEntityManager* entityManager, ComponentManager* componentManager,
                  ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        HATCHER_ASSERT(event.type == SDL_MOUSEBUTTONDOWN);
        if (event.button.button == SDL_BUTTON_RIGHT)
        {
            const Camera* camera = renderComponentManager->ReadWorldComponent<Camera>();
            const glm::vec2 worldCoords2D =
                camera->MouseCoordsToWorldCoords(event.button.x, event.button.y, frameRenderer);

            auto movementComponents = componentManager->WriteComponents<Movement2DComponent>();
            auto selectableComponents =
                renderComponentManager->ReadComponents<Selectable2DComponent>();
            auto positionComponents = componentManager->ReadComponents<Position2DComponent>();

            HATCHER_ASSERT(componentManager->Count() == renderComponentManager->Count());
            for (int i = 0; i < componentManager->Count(); i++)
            {
                std::optional<Movement2DComponent>& movementComponent = movementComponents[i];
                const std::optional<Selectable2DComponent>& selectableComponent =
                    selectableComponents[i];
                const std::optional<Position2DComponent>& positionComponent = positionComponents[i];
                if (selectableComponent && selectableComponent->selected && movementComponent)
                {
                    HATCHER_ASSERT(positionComponent);
                    std::vector<glm::vec2> path =
                        Pathfinding::GetPath(positionComponent->position, worldCoords2D,
                                             componentManager, movementComponent->obstacleOffset);
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

const int dummy = RegisterEventListener<MoveOrderEventListener>("MoveOrder");

} // namespace