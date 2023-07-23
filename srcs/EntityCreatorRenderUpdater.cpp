#include "Camera.hpp"
#include "HexagonalGrid.hpp"
#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "SelectableComponent.hpp"
#include "SteveAnimationComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IEventUpdater.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"

namespace
{

using namespace hatcher;

class EntityCreatorEventListener final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, IEntityManager* entityManager, ComponentManager* componentManager,
                  ComponentManager* renderComponentManager, const IFrameRenderer& frameRenderer) override
    {
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        HATCHER_ASSERT(event.type == SDL_MOUSEBUTTONDOWN);
        if (event.button.button == SDL_BUTTON_RIGHT && keystate[SDL_SCANCODE_LCTRL])
        {
            const Camera* camera = renderComponentManager->ReadWorldComponent<Camera>();
            const glm::vec2 worldCoords2D =
                camera->MouseCoordsToWorldCoords(event.button.x, event.button.y, frameRenderer);
            const HexagonalGrid* hexaGrid = componentManager->ReadWorldComponent<HexagonalGrid>();
            if (!hexaGrid->GetTileData(worldCoords2D).walkable)
                return;

            const glm::vec2 entitySpawnPosition = hexaGrid->GetTileCenter(worldCoords2D);
            Entity newEntity = entityManager->CreateNewEntity();
            Position2DComponent position2D{entitySpawnPosition};
            Movement2DComponent movement2D;
            movement2D.orientation = glm::vec2(1.f, 0.f);
            SelectableComponent selectable;
            selectable.selected = false;
            SteveAnimationComponent animation;
            animation.rightLegAngle = 0.f;
            animation.rightLegRising = false;

            componentManager->WriteComponents<Position2DComponent>()[newEntity] = position2D;
            componentManager->WriteComponents<Movement2DComponent>()[newEntity] = movement2D;
            renderComponentManager->WriteComponents<SelectableComponent>()[newEntity] = selectable;
            renderComponentManager->WriteComponents<SteveAnimationComponent>()[newEntity] = animation;
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

class EntityCreatorRenderUpdater final : public RenderUpdater
{
public:
    EntityCreatorRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
    {
        eventUpdater->RegisterListener(std::make_shared<EntityCreatorEventListener>());
    }

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
    }
};

RenderUpdaterRegisterer<EntityCreatorRenderUpdater> registerer;

} // namespace