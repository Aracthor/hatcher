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
#include "hatcher/EntityEgg.hpp"
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
        EntityEgg entityEgg = entityManager->CreateNewEntity(m_entityDescriptor);
        entityEgg.GetComponent<Position2DComponent>()->position = m_spawnPosition;

        std::vector<Entity::IDType> inventoryStorage;
        for (const unique_ptr<IEntityDescriptor>& itemDescriptor : m_inventoryDescriptors)
        {
            EntityEgg newItem = entityManager->CreateNewEntity(itemDescriptor.get());
            inventoryStorage.push_back(newItem.NewEntityID().ID());
        }
        if (!inventoryStorage.empty())
        {
            auto& inventoryComponent = entityEgg.GetComponent<InventoryComponent>();
            inventoryComponent->storage = inventoryStorage;
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
        m_steveEntityDescriptor = CreateEntityDescriptor(
            {
                InventoryComponent{},
                Movement2DComponent{},
                NameComponent{
                    .name = "Steve",
                },
                Position2DComponent{
                    .position = {},
                    .orientation = {1.f, 0.f},
                },
            },
            {
                SelectableComponent{
                    .box = {},
                    .selected = false,
                },
                SteveAnimationComponent{
                    .rightLegAngle = 0.f,
                    .rightLegRising = false,
                },
            });

        m_steveInventoryDescriptors.push_back(CreateEntityDescriptor(
            {
                ItemComponent{},
                NameComponent{
                    .name = "EM Card",
                },
            },
            {}));

        m_lockerEntityDescriptor = CreateEntityDescriptor(
            {
                InventoryComponent{},
                NameComponent{
                    .name = "Locker",
                },
                Position2DComponent{
                    .position = {},
                    .orientation = {1.f, 0.f},
                },
            },
            {
                SelectableComponent{
                    .box = {},
                    .selected = false,
                },
                StaticMeshComponent{
                    .type = StaticMeshComponent::Locker,
                },
            });
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