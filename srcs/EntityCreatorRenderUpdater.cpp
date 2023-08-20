#include "Camera.hpp"
#include "HexagonalGrid.hpp"
#include "InventoryComponent.hpp"
#include "ItemComponent.hpp"
#include "Movement2DComponent.hpp"
#include "NameComponent.hpp"
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
    CreateEntityCommand(const IEntityDescriptor* entityDescriptor,
                        const span<const unique_ptr<IEntityDescriptor>> inventoryDescriptors, glm::vec2 spawnPosition)
        : m_entityDescriptor(entityDescriptor)
        , m_inventoryDescriptors(inventoryDescriptors)
        , m_spawnPosition(spawnPosition)
    {
    }

    void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                 ComponentManager* renderingComponentManager) override
    {
        Entity newEntity = entityManager->CreateNewEntity(m_entityDescriptor);
        componentManager->WriteComponents<Position2DComponent>()[newEntity]->position = m_spawnPosition;

        std::vector<Entity::IDType> inventoryStorage;
        for (const unique_ptr<IEntityDescriptor>& itemDescriptor : m_inventoryDescriptors)
        {
            Entity newItem = entityManager->CreateNewEntity(itemDescriptor.get());
            inventoryStorage.push_back(newItem.ID());
        }
        if (!inventoryStorage.empty())
        {
            auto inventoryComponents = componentManager->WriteComponents<InventoryComponent>();
            HATCHER_ASSERT(inventoryComponents[newEntity]);
            inventoryComponents[newEntity]->storage = inventoryStorage;
        }
    }

private:
    const IEntityDescriptor* m_entityDescriptor;
    const span<const unique_ptr<IEntityDescriptor>> m_inventoryDescriptors;
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
            NameComponent name;
            name.name = "Steve";
            InventoryComponent inventory;

            steveEntityDescriptorBuilder.AddComponent(position2D);
            steveEntityDescriptorBuilder.AddComponent(movement2D);
            steveEntityDescriptorBuilder.AddComponent(name);
            steveEntityDescriptorBuilder.AddComponent(inventory);

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
            EntityDescriptorBuilder steveEMCardEntityDescriptorBuilder;
            ItemComponent item;
            NameComponent name;
            name.name = "EM Card";
            steveEMCardEntityDescriptorBuilder.AddComponent(item);
            steveEMCardEntityDescriptorBuilder.AddComponent(name);

            m_steveInventoryDescriptors.push_back(steveEMCardEntityDescriptorBuilder.CreateDescriptor());
        }

        {
            EntityDescriptorBuilder lockerEntityDescriptorBuilder;
            Position2DComponent position2D;
            position2D.position = {};
            position2D.orientation = glm::vec2(1.f, 0.f);
            InventoryComponent inventory;
            NameComponent name;
            name.name = "Locker";

            lockerEntityDescriptorBuilder.AddComponent(position2D);
            lockerEntityDescriptorBuilder.AddComponent(inventory);
            lockerEntityDescriptorBuilder.AddComponent(name);

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
            span<const unique_ptr<IEntityDescriptor>> inventoryDescriptors = {};
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                entityDescriptor = m_steveEntityDescriptor.get();
                inventoryDescriptors = {m_steveInventoryDescriptors};
            }
            else if (event.button.button == SDL_BUTTON_MIDDLE)
            {
                entityDescriptor = m_lockerEntityDescriptor.get();
            }

            if (entityDescriptor)
                commandManager->AddCommand(
                    new CreateEntityCommand(entityDescriptor, inventoryDescriptors, entitySpawnPosition));
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
    std::vector<unique_ptr<IEntityDescriptor>> m_steveInventoryDescriptors;
};

RenderUpdaterRegisterer<EntityCreatorRenderUpdater> registerer;

} // namespace