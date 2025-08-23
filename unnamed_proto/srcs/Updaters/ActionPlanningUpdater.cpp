#include "Components/ActionPlanningComponent.hpp"
#include "Components/InventoryComponent.hpp"
#include "Components/ItemComponent.hpp"
#include "Components/LockableComponent.hpp"
#include "Components/Movement2DComponent.hpp"
#include "Components/NameComponent.hpp"
#include "Components/Position2DComponent.hpp"
#include "Components/WorkerComponent.hpp"

#include "WorldComponents/SquareGrid.hpp"

#include "utils/EntityFinder.hpp"
#include "utils/TimeOfDay.hpp"

#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"

#include <algorithm>

using namespace hatcher;

namespace
{

const glm::vec2 woodTarget = glm::vec2(0.5f, 0.5f);

class IPlan
{
public:
    virtual bool CanBeAchieved(const ComponentAccessor* componentAccessor, Entity entity) const = 0;
    virtual void Start(IEntityManager* entityManager, ComponentAccessor* componentAccessor, Entity entity) const = 0;
    virtual bool IsOngoing(const ComponentAccessor* componentAccessor, Entity entity) const = 0;
};

bool IsEntityWood(const ComponentAccessor* componentAccessor, Entity entity)
{
    const auto nameComponent = componentAccessor->ReadComponents<NameComponent>()[entity];
    return nameComponent && nameComponent->name == "Wood";
}

bool IsEntityAxe(const ComponentAccessor* componentAccessor, Entity entity)
{
    const auto nameComponent = componentAccessor->ReadComponents<NameComponent>()[entity];
    return nameComponent && nameComponent->name == "Axe";
}

bool IsAvailableEntityAxe(const ComponentAccessor* componentAccessor, Entity entity)
{
    return IsEntityAxe(componentAccessor, entity) &&
           !componentAccessor->ReadComponents<LockableComponent>()[entity]->locker;
}

bool IsWoodStack(const ComponentAccessor* componentAccessor, Entity entity)
{
    const auto& positionComponent = componentAccessor->ReadComponents<Position2DComponent>()[entity];
    return IsEntityWood(componentAccessor, entity) && positionComponent && positionComponent->position == woodTarget;
}

bool IsGatherableWood(const ComponentAccessor* componentAccessor, Entity entity)
{
    const auto& positionComponent = componentAccessor->ReadComponents<Position2DComponent>()[entity];
    const auto& lockableComponent = componentAccessor->ReadComponents<LockableComponent>()[entity];
    return IsEntityWood(componentAccessor, entity) && positionComponent && positionComponent->position != woodTarget &&
           !lockableComponent->locker;
}

bool ContainsAxe(const ComponentAccessor* componentAccessor, Entity entity)
{
    const auto& inventoryComponent = componentAccessor->ReadComponents<InventoryComponent>()[entity];
    if (!inventoryComponent)
        return false;
    auto IsAxe = [componentAccessor](Entity entity) { return IsEntityAxe(componentAccessor, entity); };
    return std::any_of(inventoryComponent->storage.begin(), inventoryComponent->storage.end(), IsAxe);
}

bool ContainsAvailableAxe(const ComponentAccessor* componentAccessor, Entity entity)
{
    const auto& inventoryComponent = componentAccessor->ReadComponents<InventoryComponent>()[entity];
    if (!inventoryComponent)
        return false;
    auto IsAvailableAxe = [componentAccessor](Entity entity)
    { return IsAvailableEntityAxe(componentAccessor, entity); };
    return std::any_of(inventoryComponent->storage.begin(), inventoryComponent->storage.end(), IsAvailableAxe);
}

bool IsChoppableTree(const ComponentAccessor* componentAccessor, Entity entity)
{
    const auto& nameComponent = componentAccessor->ReadComponents<NameComponent>()[entity];
    const auto& lockableComponent = componentAccessor->ReadComponents<LockableComponent>()[entity];
    return nameComponent && nameComponent->name == "Tree" && !lockableComponent->locker;
}

class DropOffWood : public IPlan
{
    bool CanBeAchieved(const ComponentAccessor* componentAccessor, Entity entity) const override
    {
        const InventoryComponent& inventory = *componentAccessor->ReadComponents<InventoryComponent>()[entity];
        const glm::vec2 position = componentAccessor->ReadComponents<Position2DComponent>()[entity]->position;
        auto IsWood = [componentAccessor](Entity entity) { return IsEntityWood(componentAccessor, entity); };
        return glm::length(position - woodTarget) <= 1.f &&
               std::any_of(inventory.storage.begin(), inventory.storage.end(), IsWood);
    }

    void Start(IEntityManager* entityManager, ComponentAccessor* componentAccessor, Entity entity) const override
    {
        auto positions = componentAccessor->WriteComponents<Position2DComponent>();
        auto items = componentAccessor->WriteComponents<ItemComponent>();

        InventoryComponent& inventory = *componentAccessor->WriteComponents<InventoryComponent>()[entity];
        auto IsWood = [componentAccessor](Entity entity) { return IsEntityWood(componentAccessor, entity); };
        auto it = std::find_if(inventory.storage.begin(), inventory.storage.end(), IsWood);
        HATCHER_ASSERT(it != inventory.storage.end());

        const Entity gatherableWood = FindNearestEntity(componentAccessor, entity, IsWoodStack);
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
            items[gatherableWood]->count += items[*it]->count;
            entityManager->DeleteEntity(Entity(*it));
        }

        inventory.storage.erase(it);
    }

    bool IsOngoing(const ComponentAccessor* componentAccessor, Entity entity) const override { return false; }
};

class BringBackWood : public IPlan
{
    bool CanBeAchieved(const ComponentAccessor* componentAccessor, Entity entity) const override
    {
        const InventoryComponent& inventory = *componentAccessor->ReadComponents<InventoryComponent>()[entity];
        auto IsWood = [componentAccessor](Entity entity) { return IsEntityWood(componentAccessor, entity); };
        return std::any_of(inventory.storage.begin(), inventory.storage.end(), IsWood);
    }

    void Start(IEntityManager* entityManager, ComponentAccessor* componentAccessor, Entity entity) const override
    {
        const glm::vec2 position = componentAccessor->ReadComponents<Position2DComponent>()[entity]->position;
        const SquareGrid* grid = componentAccessor->ReadWorldComponent<SquareGrid>();

        std::vector<glm::vec2> path = grid->GetPathIfPossible(position, woodTarget, 1.f);
        componentAccessor->WriteComponents<Movement2DComponent>()[entity]->path = path;
    }

    bool IsOngoing(const ComponentAccessor* componentAccessor, Entity entity) const override
    {
        const std::vector<glm::vec2>& path = componentAccessor->ReadComponents<Movement2DComponent>()[entity]->path;
        return !path.empty();
    }
};

class TakeWood : public IPlan
{
    bool CanBeAchieved(const ComponentAccessor* componentAccessor, Entity entity) const override
    {
        const auto& positionComponents = componentAccessor->ReadComponents<Position2DComponent>();
        const Entity woodEntity = FindNearestEntity(componentAccessor, entity, IsGatherableWood);
        return woodEntity != Entity::Invalid() &&
               positionComponents[woodEntity]->position == positionComponents[entity]->position;
    }

    void Start(IEntityManager* entityManager, ComponentAccessor* componentAccessor, Entity entity) const override
    {
        const Entity woodEntity = FindNearestEntity(componentAccessor, entity, IsGatherableWood);
        InventoryComponent& inventory = *componentAccessor->WriteComponents<InventoryComponent>()[entity];
        inventory.storage.push_back(woodEntity);
        componentAccessor->WriteComponents<Position2DComponent>()[woodEntity] = {};
        componentAccessor->WriteComponents<ItemComponent>()[woodEntity]->inventory = entity;
    }

    bool IsOngoing(const ComponentAccessor* componentAccessor, Entity entity) const override { return false; }
};

class MoveToWood : public IPlan
{
    bool CanBeAchieved(const ComponentAccessor* componentAccessor, Entity entity) const override
    {
        const Entity woodEntity = FindNearestEntity(componentAccessor, entity, IsGatherableWood);
        return woodEntity != Entity::Invalid();
    }

    void Start(IEntityManager* entityManager, ComponentAccessor* componentAccessor, Entity entity) const override
    {
        const auto& positions = componentAccessor->ReadComponents<Position2DComponent>();
        const Entity woodEntity = FindNearestEntity(componentAccessor, entity, IsGatherableWood);
        const SquareGrid* grid = componentAccessor->ReadWorldComponent<SquareGrid>();

        const glm::vec2 position = positions[entity]->position;
        const glm::vec2 woodPosition = positions[woodEntity]->position;
        std::vector<glm::vec2> path = grid->GetPathIfPossible(position, woodPosition);
        componentAccessor->WriteComponents<Movement2DComponent>()[entity]->path = path;

        componentAccessor->WriteComponents<ActionPlanningComponent>()[entity]->lockedEntity = woodEntity;
        componentAccessor->WriteComponents<LockableComponent>()[woodEntity]->locker = entity;
    }

    bool IsOngoing(const ComponentAccessor* componentAccessor, Entity entity) const override
    {
        const std::vector<glm::vec2>& path = componentAccessor->ReadComponents<Movement2DComponent>()[entity]->path;
        return !path.empty();
    }
};

class ChopTree : public IPlan
{
    bool CanBeAchieved(const ComponentAccessor* componentAccessor, Entity entity) const override
    {
        if (!ContainsAxe(componentAccessor, entity))
            return false;
        const auto& positionComponents = componentAccessor->ReadComponents<Position2DComponent>();
        const Entity treeEntity = FindNearestEntity(componentAccessor, entity, IsChoppableTree);
        return treeEntity != Entity::Invalid() &&
               glm::distance(positionComponents[treeEntity]->position, positionComponents[entity]->position) <= 1.f;
    }

    void Start(IEntityManager* entityManager, ComponentAccessor* componentAccessor, Entity entity) const override
    {
        auto positionComponents = componentAccessor->WriteComponents<Position2DComponent>();
        const Entity treeEntity = FindNearestEntity(componentAccessor, entity, IsChoppableTree);

        WorkerComponent& worker = *componentAccessor->WriteComponents<WorkerComponent>()[entity];
        worker.workIndex = EWork::ChopTree;
        worker.target = treeEntity;
        worker.workedTicks = 0;
        worker.workLength = MinutesToTicks(10);

        const glm::vec2 lumberjackPosition = positionComponents[entity]->position;
        const glm::vec2 treePosition = positionComponents[treeEntity]->position;
        positionComponents[entity]->orientation = glm::normalize(treePosition - lumberjackPosition);

        componentAccessor->WriteComponents<ActionPlanningComponent>()[entity]->lockedEntity = treeEntity;
        componentAccessor->WriteComponents<LockableComponent>()[treeEntity]->locker = entity;
    }

    bool IsOngoing(const ComponentAccessor* componentAccessor, Entity entity) const override
    {
        const WorkerComponent& worker = *componentAccessor->ReadComponents<WorkerComponent>()[entity];
        return worker.workIndex.has_value();
    }
};

class MoveToTree : public IPlan
{
    bool CanBeAchieved(const ComponentAccessor* componentAccessor, Entity entity) const override
    {
        if (!ContainsAxe(componentAccessor, entity))
            return false;
        const Entity treeEntity = FindNearestEntity(componentAccessor, entity, IsChoppableTree);
        return treeEntity != Entity::Invalid();
    }

    void Start(IEntityManager* entityManager, ComponentAccessor* componentAccessor, Entity entity) const override
    {
        const auto& positions = componentAccessor->ReadComponents<Position2DComponent>();
        const Entity treeEntity = FindNearestEntity(componentAccessor, entity, IsChoppableTree);
        const SquareGrid* grid = componentAccessor->ReadWorldComponent<SquareGrid>();

        const glm::vec2 position = positions[entity]->position;
        const glm::vec2 treePosition = positions[treeEntity]->position;
        std::vector<glm::vec2> path = grid->GetPathIfPossible(position, treePosition, 1.f);
        componentAccessor->WriteComponents<Movement2DComponent>()[entity]->path = path;

        componentAccessor->WriteComponents<ActionPlanningComponent>()[entity]->lockedEntity = treeEntity;
        componentAccessor->WriteComponents<LockableComponent>()[treeEntity]->locker = entity;
    }

    bool IsOngoing(const ComponentAccessor* componentAccessor, Entity entity) const override
    {
        const std::vector<glm::vec2>& path = componentAccessor->ReadComponents<Movement2DComponent>()[entity]->path;
        return !path.empty();
    }
};

class GetAxe : public IPlan
{
    bool CanBeAchieved(const ComponentAccessor* componentAccessor, Entity entity) const override
    {
        if (ContainsAxe(componentAccessor, entity))
            return false;
        const auto& positionComponents = componentAccessor->ReadComponents<Position2DComponent>();
        const Entity rackEntity = FindNearestEntity(componentAccessor, entity, ContainsAvailableAxe);
        return rackEntity != Entity::Invalid() &&
               glm::distance(positionComponents[rackEntity]->position, positionComponents[entity]->position) <= 1.f;
    }

    void Start(IEntityManager* entityManager, ComponentAccessor* componentAccessor, Entity entity) const override
    {
        auto inventoryComponents = componentAccessor->WriteComponents<InventoryComponent>();
        const Entity rackEntity = FindNearestEntity(componentAccessor, entity, ContainsAvailableAxe);
        HATCHER_ASSERT(rackEntity != Entity::Invalid());
        auto& rackInventory = inventoryComponents[rackEntity];
        HATCHER_ASSERT(rackInventory);
        auto IsAvailableAxe = [componentAccessor](Entity entity)
        { return IsAvailableEntityAxe(componentAccessor, entity); };
        auto it = std::find_if(rackInventory->storage.begin(), rackInventory->storage.end(), IsAvailableAxe);
        HATCHER_ASSERT(it != rackInventory->storage.end());

        componentAccessor->WriteComponents<ItemComponent>()[*it]->inventory = entity;
        componentAccessor->WriteComponents<LockableComponent>()[*it]->locker = entity;
        inventoryComponents[entity]->storage.push_back(*it);
        rackInventory->storage.erase(it);
    }

    bool IsOngoing(const ComponentAccessor* componentAccessor, Entity entity) const override { return false; }
};

class MoveToAxe : public IPlan
{
    bool CanBeAchieved(const ComponentAccessor* componentAccessor, Entity entity) const override
    {
        if (ContainsAxe(componentAccessor, entity))
            return false;
        const Entity rackEntity = FindNearestEntity(componentAccessor, entity, ContainsAvailableAxe);
        return rackEntity != Entity::Invalid();
    }

    void Start(IEntityManager* entityManager, ComponentAccessor* componentAccessor, Entity entity) const override
    {
        const auto& positions = componentAccessor->ReadComponents<Position2DComponent>();
        const Entity rackEntity = FindNearestEntity(componentAccessor, entity, ContainsAvailableAxe);
        const SquareGrid* grid = componentAccessor->ReadWorldComponent<SquareGrid>();

        const glm::vec2 position = positions[entity]->position;
        const glm::vec2 treePosition = positions[rackEntity]->position;
        std::vector<glm::vec2> path = grid->GetPathIfPossible(position, treePosition, 1.f);
        componentAccessor->WriteComponents<Movement2DComponent>()[entity]->path = path;

        componentAccessor->WriteComponents<ActionPlanningComponent>()[entity]->lockedEntity = rackEntity;
    }

    bool IsOngoing(const ComponentAccessor* componentAccessor, Entity entity) const override
    {
        const std::vector<glm::vec2>& path = componentAccessor->ReadComponents<Movement2DComponent>()[entity]->path;
        return !path.empty();
    }
};

const IPlan* plans[] = {
    new DropOffWood(),
    new BringBackWood(),
    new TakeWood(),
    new MoveToWood(),
    //
    new ChopTree(),
    new MoveToTree(),
    new GetAxe(),
    new MoveToAxe(),
};

void UpdatePlanning(ActionPlanningComponent& planning, IEntityManager* entityManager,
                    ComponentAccessor* componentAccessor, Entity entity)
{
    if (!planning.currentActionIndex || !plans[*planning.currentActionIndex]->IsOngoing(componentAccessor, entity))
    {
        if (planning.lockedEntity)
        {
            auto& lockable = componentAccessor->WriteComponents<LockableComponent>()[*planning.lockedEntity];
            HATCHER_ASSERT(lockable);
            lockable->locker = {};
            planning.lockedEntity = {};
        }
        planning.currentActionIndex = {};
        for (int planIndex = 0; planIndex < (int)std::size(plans); planIndex++)
        {
            const IPlan* plan = plans[planIndex];
            if (plan->CanBeAchieved(componentAccessor, entity))
            {
                planning.currentActionIndex = planIndex;
                plan->Start(entityManager, componentAccessor, entity);
                break;
            }
        }
    }
}

class ActionPlanningUpdater final : public Updater
{
    void Update(IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        auto plannings = componentAccessor->WriteComponents<ActionPlanningComponent>();

        for (int i = 0; i < componentAccessor->Count(); i++)
        {
            if (plannings[i])
            {
                ActionPlanningComponent& planning = *plannings[i];
                UpdatePlanning(planning, entityManager, componentAccessor, Entity(i));
            }
        }
    }

    void OnDeletedEntity(Entity entity, IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        {
            const auto& planning = componentAccessor->ReadComponents<ActionPlanningComponent>()[entity];
            if (planning && planning->lockedEntity)
            {
                auto& lockable = componentAccessor->WriteComponents<LockableComponent>()[*planning->lockedEntity];
                HATCHER_ASSERT(lockable);
                lockable->locker = {};
            }
        }
        {
            const auto& lockable = componentAccessor->ReadComponents<LockableComponent>()[entity];
            if (lockable && lockable->locker)
            {
                auto& planning = componentAccessor->WriteComponents<ActionPlanningComponent>()[*lockable->locker];
                planning->currentActionIndex = {};
                planning->lockedEntity = {};
            }
        }
    }
};

UpdaterRegisterer<ActionPlanningUpdater> registerer;

} // namespace
