#include "Components/InventoryComponent.hpp"
#include "Components/Position2DComponent.hpp"
#include "WorldComponents/Camera.hpp"
#include "WorldComponents/HexagonalGrid.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityDescriptorID.hpp"
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
    CreateEntityCommand(const EntityDescriptorID& entityDescriptor, span<const EntityDescriptorID> inventoryDescriptors,
                        glm::vec2 spawnPosition)
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
        for (const EntityDescriptorID& itemDescriptor : m_inventoryDescriptors)
        {
            EntityEgg newItem = entityManager->CreateNewEntity(itemDescriptor);
            inventoryStorage.push_back(newItem.NewEntityID().ID());
        }
        if (!inventoryStorage.empty())
        {
            auto& inventoryComponent = entityEgg.GetComponent<InventoryComponent>();
            inventoryComponent->storage = inventoryStorage;
        }
    }

private:
    const EntityDescriptorID m_entityDescriptor;
    const span<const EntityDescriptorID> m_inventoryDescriptors;
    const glm::vec2 m_spawnPosition;
};

class EntityCreatorRenderUpdater final : public RenderUpdater
{
public:
    EntityCreatorRenderUpdater(const IRendering* rendering)
        : m_steveEntityDescriptor(EntityDescriptorID::Create("Steve"))
        , m_lockerEntityDescriptor(EntityDescriptorID::Create("Locker"))
        , m_melonEntityDescriptor(EntityDescriptorID::Create("Melon"))
    {
        m_steveInventoryDescriptors.push_back(EntityDescriptorID::Create("EMCard"));
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
            EntityDescriptorID* entityDescriptor = nullptr;
            span<const EntityDescriptorID> inventoryDescriptors = {};
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                entityDescriptor = &m_steveEntityDescriptor;
                inventoryDescriptors = {m_steveInventoryDescriptors};
            }
            else if (event.button.button == SDL_BUTTON_MIDDLE)
            {
                entityDescriptor = &m_lockerEntityDescriptor;
            }
            else if (event.button.button == SDL_BUTTON_LEFT)
            {
                entityDescriptor = &m_melonEntityDescriptor;
            }

            if (entityDescriptor)
                commandManager->AddCommand(
                    new CreateEntityCommand(*entityDescriptor, inventoryDescriptors, entitySpawnPosition));
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
    EntityDescriptorID m_steveEntityDescriptor;
    EntityDescriptorID m_lockerEntityDescriptor;
    EntityDescriptorID m_melonEntityDescriptor;
    std::vector<EntityDescriptorID> m_steveInventoryDescriptors;
};

RenderUpdaterRegisterer<EntityCreatorRenderUpdater> registerer;

} // namespace