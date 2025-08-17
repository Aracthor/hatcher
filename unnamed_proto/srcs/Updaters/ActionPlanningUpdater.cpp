#include "Components/ActionPlanningComponent.hpp"
#include "Components/InventoryComponent.hpp"
#include "Components/ItemComponent.hpp"
#include "Components/Movement2DComponent.hpp"
#include "Components/NameComponent.hpp"
#include "Components/Position2DComponent.hpp"

#include "WorldComponents/SquareGrid.hpp"

#include "utils/EntityFinder.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Updater.hpp"

#include <algorithm>

using namespace hatcher;

namespace
{

const glm::vec2 woodTarget = glm::vec2(0.5f, 0.5f);

class IPlan
{
public:
    virtual bool CanBeAchieved(const ComponentManager* componentManager, Entity entity) const = 0;
    virtual void Start(ComponentManager* componentManager, Entity entity) const = 0;
    virtual bool IsOngoing(const ComponentManager* componentManager, Entity entity) const = 0;
};

bool IsEntityWood(const ComponentManager* componentManager, Entity entity)
{
    return componentManager->ReadComponents<NameComponent>()[entity]->name == "Wood";
}

bool IsWoodStack(const ComponentManager* componentManager, Entity entity)
{
    const auto& positionComponent = componentManager->ReadComponents<Position2DComponent>()[entity];
    return IsEntityWood(componentManager, entity) && positionComponent && positionComponent->position == woodTarget;
}

bool IsGatherableWood(const ComponentManager* componentManager, Entity entity)
{
    const auto& positionComponent = componentManager->ReadComponents<Position2DComponent>()[entity];
    return IsEntityWood(componentManager, entity) && positionComponent && positionComponent->position != woodTarget;
}

class DropOffWood : public IPlan
{
    bool CanBeAchieved(const ComponentManager* componentManager, Entity entity) const override
    {
        const InventoryComponent& inventory = *componentManager->ReadComponents<InventoryComponent>()[entity];
        const glm::vec2 position = componentManager->ReadComponents<Position2DComponent>()[entity]->position;
        auto IsWood = [componentManager](Entity entity) { return IsEntityWood(componentManager, entity); };
        return glm::length(position - woodTarget) <= 1.f &&
               std::any_of(inventory.storage.begin(), inventory.storage.end(), IsWood);
    }

    void Start(ComponentManager* componentManager, Entity entity) const override
    {
        auto positions = componentManager->WriteComponents<Position2DComponent>();
        auto items = componentManager->WriteComponents<ItemComponent>();

        InventoryComponent& inventory = *componentManager->WriteComponents<InventoryComponent>()[entity];
        auto IsWood = [componentManager](Entity entity) { return IsEntityWood(componentManager, entity); };
        auto it = std::find_if(inventory.storage.begin(), inventory.storage.end(), IsWood);
        HATCHER_ASSERT(it != inventory.storage.end());

        const Entity gatherableWood = FindNearestEntity(componentManager, entity, IsWoodStack);
        if (gatherableWood == Entity::Invalid())
        {
            items[*it]->inventory = Entity::Invalid();
            positions[*it] = Position2DComponent{
                .position = woodTarget,
                .orientation = {1.f, 0.f},
            };
        }
        else
        {
            items[gatherableWood]->count += 1;
            componentManager->RemoveEntity(Entity(*it));
        }

        inventory.storage.erase(it);
    }

    bool IsOngoing(const ComponentManager* componentManager, Entity entity) const override { return false; }
};

class BringBackWood : public IPlan
{
    bool CanBeAchieved(const ComponentManager* componentManager, Entity entity) const override
    {
        const InventoryComponent& inventory = *componentManager->ReadComponents<InventoryComponent>()[entity];
        auto IsWood = [componentManager](Entity entity) { return IsEntityWood(componentManager, entity); };
        return std::any_of(inventory.storage.begin(), inventory.storage.end(), IsWood);
    }

    void Start(ComponentManager* componentManager, Entity entity) const override
    {
        const glm::vec2 position = componentManager->ReadComponents<Position2DComponent>()[entity]->position;
        const SquareGrid* grid = componentManager->ReadWorldComponent<SquareGrid>();

        std::vector<glm::vec2> path = grid->GetPathIfPossible(position, woodTarget, 1.f);
        componentManager->WriteComponents<Movement2DComponent>()[entity]->path = path;
    }

    bool IsOngoing(const ComponentManager* componentManager, Entity entity) const override
    {
        const std::vector<glm::vec2>& path = componentManager->ReadComponents<Movement2DComponent>()[entity]->path;
        return !path.empty();
    }
};

class TakeWood : public IPlan
{
    bool CanBeAchieved(const ComponentManager* componentManager, Entity entity) const override
    {
        const auto& positionComponents = componentManager->ReadComponents<Position2DComponent>();
        const Entity woodEntity = FindNearestEntity(componentManager, entity, IsGatherableWood);
        return woodEntity != Entity::Invalid() &&
               positionComponents[woodEntity]->position == positionComponents[entity]->position;
    }

    void Start(ComponentManager* componentManager, Entity entity) const override
    {
        const Entity woodEntity = FindNearestEntity(componentManager, entity, IsGatherableWood);
        InventoryComponent& inventory = *componentManager->WriteComponents<InventoryComponent>()[entity];
        inventory.storage.push_back(woodEntity);
        componentManager->WriteComponents<Position2DComponent>()[woodEntity] = {};
        componentManager->WriteComponents<ItemComponent>()[woodEntity]->inventory = entity;
    }

    bool IsOngoing(const ComponentManager* componentManager, Entity entity) const override { return false; }
};

class MoveToWood : public IPlan
{
    bool CanBeAchieved(const ComponentManager* componentManager, Entity entity) const override
    {
        const Entity woodEntity = FindNearestEntity(componentManager, entity, IsGatherableWood);
        return woodEntity != Entity::Invalid();
    }

    void Start(ComponentManager* componentManager, Entity entity) const override
    {
        const auto& positions = componentManager->ReadComponents<Position2DComponent>();
        const Entity woodEntity = FindNearestEntity(componentManager, entity, IsGatherableWood);
        const SquareGrid* grid = componentManager->ReadWorldComponent<SquareGrid>();

        const glm::vec2 position = positions[entity]->position;
        const glm::vec2 woodPosition = positions[woodEntity]->position;
        std::vector<glm::vec2> path = grid->GetPathIfPossible(position, woodPosition);
        componentManager->WriteComponents<Movement2DComponent>()[entity]->path = path;
    }

    bool IsOngoing(const ComponentManager* componentManager, Entity entity) const override
    {
        const std::vector<glm::vec2>& path = componentManager->ReadComponents<Movement2DComponent>()[entity]->path;
        return !path.empty();
    }
};

const IPlan* plans[] = {
    new DropOffWood(),
    new BringBackWood(),
    new TakeWood(),
    new MoveToWood(),
};

void UpdatePlanning(ActionPlanningComponent& planning, ComponentManager* componentManager, Entity entity)
{
    if (!planning.currentActionIndex || !plans[*planning.currentActionIndex]->IsOngoing(componentManager, entity))
    {
        planning.currentActionIndex = {};
        for (int planIndex = 0; planIndex < (int)std::size(plans); planIndex++)
        {
            const IPlan* plan = plans[planIndex];
            if (plan->CanBeAchieved(componentManager, entity))
            {
                planning.currentActionIndex = planIndex;
                plan->Start(componentManager, entity);
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
                UpdatePlanning(planning, componentManager, Entity(i));
            }
        }
    }
};

UpdaterRegisterer<ActionPlanningUpdater> registerer;

} // namespace
