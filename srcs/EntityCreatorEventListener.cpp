#include "Camera.hpp"
#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "Selectable2DComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"

namespace
{

using namespace hatcher;

class EntityCreatorEventListener final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, IEntityManager* entityManager,
                  ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        HATCHER_ASSERT(event.type == SDL_MOUSEBUTTONDOWN);
        if (event.button.button == SDL_BUTTON_MIDDLE)
        {
            const Camera* camera = renderComponentManager->ReadWorldComponent<Camera>();
            const glm::vec2 worldCoords2D =
                camera->MouseCoordsToWorldCoords(event.button.x, event.button.y, frameRenderer);

            Entity newEntity = entityManager->CreateNewEntity();
            Position2DComponent position2D{worldCoords2D};
            Movement2DComponent movement2D;
            movement2D.orientation = glm::vec2(1.f, 0.f);
            movement2D.speed = 0.f;
            movement2D.obstacleOffset = 0.5f;
            Selectable2DComponent selectable2D;
            selectable2D.selected = false;
            selectable2D.box = Box3f(glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.5f, 0.5f, 1.f));

            componentManager->AttachComponent<Position2DComponent>(newEntity, position2D);
            componentManager->AttachComponent<Movement2DComponent>(newEntity, movement2D);
            renderComponentManager->AttachComponent<Selectable2DComponent>(newEntity, selectable2D);
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

const int dummy = RegisterEventListener<EntityCreatorEventListener>("EntityCreator");

} // namespace