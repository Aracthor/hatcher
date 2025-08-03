#include "Components/InventoryComponent.hpp"
#include "Components/Position2DComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Updater.hpp"

using namespace hatcher;

namespace
{

class InventoryUpdater final : public Updater
{
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentManager* componentManager) override {}

    void OnDeletedEntity(Entity entity, WorldSettings& settings, IEntityManager* entityManager,
                         ComponentManager* componentManager) override
    {
        auto positionComponents = componentManager->WriteComponents<Position2DComponent>();
        auto inventoryComponent = componentManager->ReadComponents<InventoryComponent>()[entity];
        if (inventoryComponent)
        {
            HATCHER_ASSERT(positionComponents[entity]);
            const glm::vec2 position = positionComponents[entity]->position;
            for (Entity::IDType itemID : inventoryComponent->storage)
            {
                positionComponents[itemID] = Position2DComponent{
                    .position = position,
                    .orientation = {1.f, 0.f},
                };
            }
        }
    }
};

UpdaterRegisterer<InventoryUpdater> registerer;

} // namespace
