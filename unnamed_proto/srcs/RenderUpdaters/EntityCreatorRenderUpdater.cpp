#include "Components/InventoryComponent.hpp"
#include "Components/ItemComponent.hpp"
#include "Components/Position2DComponent.hpp"
#include "WorldComponents/Camera.hpp"
#include "WorldComponents/SquareGrid.hpp"

#include "hatcher/CommandRegisterer.hpp"
#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/EntityDescriptorID.hpp"
#include "hatcher/EntityEgg.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
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
        , m_inventoryDescriptors(inventoryDescriptors.begin(), inventoryDescriptors.end())
        , m_spawnPosition(spawnPosition)
    {
    }

    void Save(DataSaver& saver) const override
    {
        saver << m_entityDescriptor;
        saver << m_inventoryDescriptors;
        saver << m_spawnPosition;
    }

    void Load(DataLoader& loader) override
    {
        loader >> m_entityDescriptor;
        loader >> m_inventoryDescriptors;
        loader >> m_spawnPosition;
    }

    void Execute(IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        EntityEgg entityEgg = entityManager->CreateNewEntity(m_entityDescriptor);
        entityEgg.GetComponent<Position2DComponent>() = Position2DComponent();
        entityEgg.GetComponent<Position2DComponent>()->position = m_spawnPosition;

        std::vector<Entity> inventoryStorage;
        for (const EntityDescriptorID& itemDescriptor : m_inventoryDescriptors)
        {
            EntityEgg newItem = entityManager->CreateNewEntity(itemDescriptor);
            newItem.GetComponent<ItemComponent>()->inventory = entityEgg.NewEntityID();
            inventoryStorage.push_back(newItem.NewEntityID());
        }
        if (!inventoryStorage.empty())
        {
            auto& inventoryComponent = entityEgg.GetComponent<InventoryComponent>();
            inventoryComponent->storage = inventoryStorage;
        }
    }

private:
    EntityDescriptorID m_entityDescriptor;
    std::vector<EntityDescriptorID> m_inventoryDescriptors;
    glm::vec2 m_spawnPosition;

    COMMAND_HEADER(CreateEntityCommand)
};
REGISTER_COMMAND(CreateEntityCommand);

class EntityCreatorEventListener : public IEventListener
{
public:
    EntityCreatorEventListener()
        : m_steveEntityDescriptor(EntityDescriptorID::Create("Steve"))
        , m_treeEntityDescriptor(EntityDescriptorID::Create("Tree"))
        , m_woodEntityDescriptor(EntityDescriptorID::Create("Wood"))
    {
        m_steveInventoryDescriptors.push_back(EntityDescriptorID::Create("Axe"));
    }

    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentAccessor* componentAccessor, ComponentAccessor* renderComponentAccessor,
                  const IFrameRenderer& frameRenderer) override
    {
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if (event.type == SDL_MOUSEBUTTONDOWN && keystate[SDL_SCANCODE_LCTRL])
        {
            const Camera* camera = renderComponentAccessor->ReadWorldComponent<Camera>();
            const glm::vec2 worldCoords2D =
                camera->MouseCoordsToWorldCoords(event.button.x, event.button.y, frameRenderer);
            const SquareGrid* grid = componentAccessor->ReadWorldComponent<SquareGrid>();
            if (!grid->GetTileData(worldCoords2D).walkable)
                return;

            const glm::vec2 entitySpawnPosition = grid->GetTileCenter(worldCoords2D);
            EntityDescriptorID* entityDescriptor = nullptr;
            span<const EntityDescriptorID> inventoryDescriptors = {};
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                entityDescriptor = &m_steveEntityDescriptor;
                inventoryDescriptors = {m_steveInventoryDescriptors};
            }
            else if (event.button.button == SDL_BUTTON_MIDDLE)
            {
                entityDescriptor = &m_treeEntityDescriptor;
            }
            else if (event.button.button == SDL_BUTTON_LEFT)
            {
                entityDescriptor = &m_woodEntityDescriptor;
            }

            if (entityDescriptor)
                commandManager->AddCommand(
                    new CreateEntityCommand(*entityDescriptor, inventoryDescriptors, entitySpawnPosition));
        }
    }

private:
    EntityDescriptorID m_steveEntityDescriptor;
    EntityDescriptorID m_treeEntityDescriptor;
    EntityDescriptorID m_woodEntityDescriptor;
    std::vector<EntityDescriptorID> m_steveInventoryDescriptors;
};

EventListenerRegisterer<EntityCreatorEventListener> registerer;

} // namespace