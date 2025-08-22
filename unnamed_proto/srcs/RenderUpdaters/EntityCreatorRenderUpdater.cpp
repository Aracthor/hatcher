#include "Components/InventoryComponent.hpp"
#include "Components/ItemComponent.hpp"
#include "Components/Position2DComponent.hpp"
#include "RenderComponents/ItemDisplayComponent.hpp"
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
                        const ItemDisplayComponent::Locations& itemLocations, glm::vec2 spawnPosition)
        : m_entityDescriptor(entityDescriptor)
        , m_inventoryDescriptors(inventoryDescriptors.begin(), inventoryDescriptors.end())
        , m_locations(itemLocations)
        , m_spawnPosition(spawnPosition)
    {
    }

    void Save(DataSaver& saver) const override
    {
        saver << m_entityDescriptor;
        saver << m_inventoryDescriptors;
        saver << m_spawnPosition;
        saver << m_locations;
    }

    void Load(DataLoader& loader) override
    {
        loader >> m_entityDescriptor;
        loader >> m_inventoryDescriptors;
        loader >> m_spawnPosition;
        loader >> m_locations;
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
        if (!m_locations.empty())
        {
            entityEgg.GetRenderingComponent<ItemDisplayComponent>()->locations = m_locations;
        }
    }

private:
    EntityDescriptorID m_entityDescriptor;
    std::vector<EntityDescriptorID> m_inventoryDescriptors;
    ItemDisplayComponent::Locations m_locations;
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
        , m_rackEntityDescriptor(EntityDescriptorID::Create("Rack"))
        , m_hutEntityDescriptor(EntityDescriptorID::Create("LoggingHut"))
        , m_rackItemLocations(10, &ItemDisplayComponent::LocationHash)
    {
        m_rackInventoryDescriptors.push_back(EntityDescriptorID::Create("Axe"));
        m_rackInventoryDescriptors.push_back(EntityDescriptorID::Create("Axe"));
        m_rackInventoryDescriptors.push_back(EntityDescriptorID::Create("Axe"));

        glm::mat4 rackToolLocation(1.f);
        rackToolLocation = glm::translate(rackToolLocation, glm::vec3(0.08f, 0.3f, 0.f));
        rackToolLocation = glm::rotate(rackToolLocation, glm::radians(-10.f), glm::vec3(0.f, 1.f, 0.f));
        m_rackItemLocations[std::make_pair(ItemComponent::Tool, 0)] = rackToolLocation;
        rackToolLocation = glm::translate(rackToolLocation, glm::vec3(0.f, -0.2f, 0.f));
        m_rackItemLocations[std::make_pair(ItemComponent::Tool, 1)] = rackToolLocation;
        rackToolLocation = glm::translate(rackToolLocation, glm::vec3(0.f, -0.2f, 0.f));
        m_rackItemLocations[std::make_pair(ItemComponent::Tool, 2)] = rackToolLocation;
        rackToolLocation = glm::translate(rackToolLocation, glm::vec3(0.f, -0.2f, 0.f));
        m_rackItemLocations[std::make_pair(ItemComponent::Tool, 3)] = rackToolLocation;
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
            ItemDisplayComponent::Locations locations(10, &ItemDisplayComponent::LocationHash);
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
                entityDescriptor = &m_rackEntityDescriptor;
                inventoryDescriptors = {m_rackInventoryDescriptors};
                locations = m_rackItemLocations;
            }

            if (entityDescriptor)
                commandManager->AddCommand(
                    new CreateEntityCommand(*entityDescriptor, inventoryDescriptors, locations, entitySpawnPosition));
        }
    }

private:
    EntityDescriptorID m_steveEntityDescriptor;
    EntityDescriptorID m_treeEntityDescriptor;
    EntityDescriptorID m_rackEntityDescriptor;
    EntityDescriptorID m_hutEntityDescriptor;
    std::vector<EntityDescriptorID> m_steveInventoryDescriptors;
    std::vector<EntityDescriptorID> m_rackInventoryDescriptors;
    ItemDisplayComponent::Locations m_rackItemLocations;
};

EventListenerRegisterer<EntityCreatorEventListener> registerer;

} // namespace