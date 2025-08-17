#include "Components/HarvestableComponent.hpp"
#include "Components/ItemComponent.hpp"
#include "Components/Position2DComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Updater.hpp"

using namespace hatcher;

namespace
{

class HarvestableUpdater final : public Updater
{
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentManager* componentManager) override {}

    void OnDeletedEntity(Entity entity, WorldSettings& settings, IEntityManager* entityManager,
                         ComponentManager* componentManager) override
    {
        const auto harvestableComponent = componentManager->ReadComponents<HarvestableComponent>()[entity];
        const auto positionComponent = componentManager->ReadComponents<Position2DComponent>()[entity];
        if (harvestableComponent)
        {
            HATCHER_ASSERT(positionComponent);
            EntityEgg item = entityManager->CreateNewEntity(harvestableComponent->harvest);
            item.GetComponent<Position2DComponent>() = *positionComponent;
            if (harvestableComponent->amount > 1)
            {
                HATCHER_ASSERT(item.GetComponent<ItemComponent>());
                item.GetComponent<ItemComponent>()->count = harvestableComponent->amount;
            }
        }
    }
};

UpdaterRegisterer<HarvestableUpdater> registerer;

} // namespace
