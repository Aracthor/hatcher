#include "Components/ActionPlanningComponent.hpp"
#include "Components/InventoryComponent.hpp"
#include "Components/NameComponent.hpp"
#include "Components/Position2DComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Updater.hpp"

#include <algorithm>

using namespace hatcher;

namespace
{

class IPlan
{
public:
    virtual bool CanBeAchieved(const ComponentManager* componentManager, int entityIndex) const = 0;
    virtual void Start(ComponentManager* componentManager, int entityIndex) const = 0;
    virtual bool IsOngoing(const ComponentManager* componentManager, int entityIndex) const = 0;
};

bool IsEntityWood(const ComponentManager* componentManager, Entity::IDType entity)
{
    return componentManager->ReadComponents<NameComponent>()[entity]->name == "Wood";
}

class DropOffWood : public IPlan
{
    bool CanBeAchieved(const ComponentManager* componentManager, int entityIndex) const override
    {
        const InventoryComponent& inventory = *componentManager->ReadComponents<InventoryComponent>()[entityIndex];
        auto IsWood = [componentManager](Entity::IDType entity) { return IsEntityWood(componentManager, entity); };
        return std::any_of(inventory.storage.begin(), inventory.storage.end(), IsWood);
    }

    void Start(ComponentManager* componentManager, int entityIndex) const override
    {
        auto positions = componentManager->WriteComponents<Position2DComponent>();
        InventoryComponent& inventory = *componentManager->WriteComponents<InventoryComponent>()[entityIndex];
        auto IsWood = [componentManager](Entity::IDType entity) { return IsEntityWood(componentManager, entity); };
        auto it = std::find_if(inventory.storage.begin(), inventory.storage.end(), IsWood);
        HATCHER_ASSERT(it != inventory.storage.end());
        const glm::vec2 position = positions[entityIndex]->position;
        positions[*it] = Position2DComponent{
            .position = position,
            .orientation = {1.f, 0.f},
        };
        inventory.storage.erase(it);
    }

    bool IsOngoing(const ComponentManager* componentManager, int entityIndex) const override { return false; }
};

const IPlan* plans[] = {
    new DropOffWood(),
};

void UpdatePlanning(ActionPlanningComponent& planning, ComponentManager* componentManager, int entityIndex)
{
    if (!planning.currentActionIndex || !plans[*planning.currentActionIndex]->IsOngoing(componentManager, entityIndex))
    {
        planning.currentActionIndex = {};
        for (int planIndex = 0; planIndex < (int)std::size(plans); planIndex++)
        {
            const IPlan* plan = plans[planIndex];
            if (plan->CanBeAchieved(componentManager, entityIndex))
            {
                planning.currentActionIndex = planIndex;
                plan->Start(componentManager, entityIndex);
                break;
            }
        }
    }
}

class ActionPlanningUpdater final : public Updater
{
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        auto plannings = componentManager->WriteComponents<ActionPlanningComponent>();

        for (int i = 0; i < componentManager->Count(); i++)
        {
            if (plannings[i])
            {
                ActionPlanningComponent& planning = *plannings[i];
                UpdatePlanning(planning, componentManager, i);
            }
        }
    }
};

UpdaterRegisterer<ActionPlanningUpdater> registerer;

} // namespace
