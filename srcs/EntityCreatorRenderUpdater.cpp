#include "Camera.hpp"
#include "HexagonalGrid.hpp"
#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "SelectableComponent.hpp"
#include "SteveAnimationComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/ICommand.hpp"
#include "hatcher/ICommandManager.hpp"

namespace
{

using namespace hatcher;

class CreateEntityCommand final : public ICommand
{
public:
    CreateEntityCommand(glm::vec2 spawnPosition)
        : m_spawnPosition(spawnPosition)
    {
    }

    void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                 ComponentManager* renderingComponentManager) override
    {
        Entity newEntity = entityManager->CreateNewEntity();
        Position2DComponent position2D{m_spawnPosition};
        Movement2DComponent movement2D;
        movement2D.orientation = glm::vec2(1.f, 0.f);

        componentManager->WriteComponents<Position2DComponent>()[newEntity] = position2D;
        componentManager->WriteComponents<Movement2DComponent>()[newEntity] = movement2D;

        if (renderingComponentManager)
        {
            SelectableComponent selectable;
            selectable.selected = false;
            SteveAnimationComponent animation;
            animation.rightLegAngle = 0.f;
            animation.rightLegRising = false;

            renderingComponentManager->WriteComponents<SelectableComponent>()[newEntity] = selectable;
            renderingComponentManager->WriteComponents<SteveAnimationComponent>()[newEntity] = animation;
        }
    }

private:
    const glm::vec2 m_spawnPosition;
};

class EntityCreatorRenderUpdater final : public RenderUpdater
{
public:
    EntityCreatorRenderUpdater(const IRendering* rendering) {}

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
    }

    void GetEvent(const SDL_Event& event, ICommandManager* commandManager, const ComponentManager* componentManager,
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
            commandManager->AddCommand(new CreateEntityCommand(entitySpawnPosition));
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

RenderUpdaterRegisterer<EntityCreatorRenderUpdater> registerer;

} // namespace