#include "Components/GrowableComponent.hpp"
#include "Components/HarvestableComponent.hpp"
#include "Components/ItemComponent.hpp"
#include "Components/Position2DComponent.hpp"

#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Updater.hpp"

using namespace hatcher;

namespace
{

class HarvestableUpdater final : public Updater
{
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
    }

    void OnDeletedEntity(Entity entity, WorldSettings& settings, IEntityManager* entityManager,
                         ComponentAccessor* componentAccessor) override
    {
        const auto harvestableComponent = componentAccessor->ReadComponents<HarvestableComponent>()[entity];
        if (harvestableComponent)
        {
            const auto positionComponent = componentAccessor->ReadComponents<Position2DComponent>()[entity];
            const auto growableComponent = componentAccessor->ReadComponents<GrowableComponent>()[entity];
            int amount = harvestableComponent->amount;
            if (growableComponent)
                amount *= growableComponent->maturity;

            if (amount > 0)
            {
                HATCHER_ASSERT(positionComponent);
                EntityEgg item = entityManager->CreateNewEntity(harvestableComponent->harvest);
                item.GetComponent<Position2DComponent>() = *positionComponent;
                if (amount > 1)
                {
                    HATCHER_ASSERT(item.GetComponent<ItemComponent>());
                    item.GetComponent<ItemComponent>()->count = amount;
                }
            }
        }
    }
};

UpdaterRegisterer<HarvestableUpdater> registerer;

} // namespace
