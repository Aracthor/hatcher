#include "Components/InventoryComponent.hpp"
#include "Components/Position2DComponent.hpp"

#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/Updater.hpp"

using namespace hatcher;

namespace
{

class InventoryUpdater final : public Updater
{
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
    }

    void OnDeletedEntity(Entity entity, WorldSettings& settings, IEntityManager* entityManager,
                         ComponentAccessor* componentAccessor) override
    {
        auto positionComponents = componentAccessor->WriteComponents<Position2DComponent>();
        auto inventoryComponent = componentAccessor->ReadComponents<InventoryComponent>()[entity];
        if (inventoryComponent)
        {
            HATCHER_ASSERT(positionComponents[entity]);
            const glm::vec2 position = positionComponents[entity]->position;
            for (Entity itemID : inventoryComponent->storage)
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
