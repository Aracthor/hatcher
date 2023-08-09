#include "Camera.hpp"
#include "HexagonalGrid.hpp"
#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "SelectableComponent.hpp"
#include "StaticMeshComponent.hpp"
#include "SteveAnimationComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityDescriptor.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/ICommand.hpp"
#include "hatcher/ICommandManager.hpp"
#include "hatcher/unique_ptr.hpp"

namespace
{

using namespace hatcher;

class CreateEntityCommand final : public ICommand
{
public:
    CreateEntityCommand(const IEntityDescriptor* entityDescriptor, glm::vec2 spawnPosition)
        : m_entityDescriptor(entityDescriptor)
        , m_spawnPosition(spawnPosition)
    {
    }

    void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                 ComponentManager* renderingComponentManager) override
    {
        Entity newEntity = entityManager->CreateNewEntity(m_entityDescriptor);
        componentManager->WriteComponents<Position2DComponent>()[newEntity]->position = m_spawnPosition;
    }

private:
    const IEntityDescriptor* m_entityDescriptor;
    const glm::vec2 m_spawnPosition;
};

class EntityCreatorRenderUpdater final : public RenderUpdater
{
public:
    EntityCreatorRenderUpdater(const IRendering* rendering)
    {
        {
            EntityDescriptorBuilder steveEntityDescriptorBuilder;
            Position2DComponent position2D;
            position2D.position = {};
            position2D.orientation = glm::vec2(1.f, 0.f);
            Movement2DComponent movement2D;

            steveEntityDescriptorBuilder.AddComponent(position2D);
            steveEntityDescriptorBuilder.AddComponent(movement2D);

            SelectableComponent selectable;
            selectable.selected = false;
            selectable.box = {};
            SteveAnimationComponent animation;
            animation.rightLegAngle = 0.f;
            animation.rightLegRising = false;

            steveEntityDescriptorBuilder.AddRenderingComponent(selectable);
            steveEntityDescriptorBuilder.AddRenderingComponent(animation);

            m_steveEntityDescriptor = steveEntityDescriptorBuilder.CreateDescriptor();
        }
        {
            EntityDescriptorBuilder lockerEntityDescriptorBuilder;
            Position2DComponent position2D;
            position2D.position = {};
            position2D.orientation = glm::vec2(1.f, 0.f);

            lockerEntityDescriptorBuilder.AddComponent(position2D);

            SelectableComponent selectable;
            selectable.selected = false;
            selectable.box = {};
            StaticMeshComponent staticMesh;
            staticMesh.type = StaticMeshComponent::Locker;

            lockerEntityDescriptorBuilder.AddRenderingComponent(selectable);
            lockerEntityDescriptorBuilder.AddRenderingComponent(staticMesh);

            m_lockerEntityDescriptor = lockerEntityDescriptorBuilder.CreateDescriptor();
        }
    }

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
    }

    void GetEvent(const SDL_Event& event, ICommandManager* commandManager, const ComponentManager* componentManager,
                  ComponentManager* renderComponentManager, const IFrameRenderer& frameRenderer) override
    {
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        HATCHER_ASSERT(event.type == SDL_MOUSEBUTTONDOWN);
        if (keystate[SDL_SCANCODE_LCTRL])
        {
            const Camera* camera = renderComponentManager->ReadWorldComponent<Camera>();
            const glm::vec2 worldCoords2D =
                camera->MouseCoordsToWorldCoords(event.button.x, event.button.y, frameRenderer);
            const HexagonalGrid* hexaGrid = componentManager->ReadWorldComponent<HexagonalGrid>();
            if (!hexaGrid->GetTileData(worldCoords2D).walkable)
                return;

            const glm::vec2 entitySpawnPosition = hexaGrid->GetTileCenter(worldCoords2D);
            const IEntityDescriptor* entityDescriptor = nullptr;
            if (event.button.button == SDL_BUTTON_RIGHT)
                entityDescriptor = m_steveEntityDescriptor.get();
            else if (event.button.button == SDL_BUTTON_MIDDLE)
                entityDescriptor = m_lockerEntityDescriptor.get();

            commandManager->AddCommand(new CreateEntityCommand(entityDescriptor, entitySpawnPosition));
        }
    }

    span<const SDL_EventType> EventTypesToListen() const override
    {
        static const SDL_EventType events[] = {
            SDL_MOUSEBUTTONDOWN,
        };
        return span<const SDL_EventType>(events, std::size(events));
    }

private:
    unique_ptr<IEntityDescriptor> m_steveEntityDescriptor;
    unique_ptr<IEntityDescriptor> m_lockerEntityDescriptor;
};

RenderUpdaterRegisterer<EntityCreatorRenderUpdater> registerer;

} // namespace