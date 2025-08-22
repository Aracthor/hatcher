#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/EntityDescriptor.hpp"

#include "Components/ActionPlanningComponent.hpp"
#include "Components/GrowableComponent.hpp"
#include "Components/HarvestableComponent.hpp"
#include "Components/InventoryComponent.hpp"
#include "Components/ItemComponent.hpp"
#include "Components/LockableComponent.hpp"
#include "Components/Movement2DComponent.hpp"
#include "Components/NameComponent.hpp"
#include "Components/ObstacleComponent.hpp"
#include "Components/Position2DComponent.hpp"
#include "Components/WorkerComponent.hpp"
#include "RenderComponents/ItemDisplayComponent.hpp"
#include "RenderComponents/SelectableComponent.hpp"
#include "RenderComponents/StaticMeshComponent.hpp"
#include "RenderComponents/SteveAnimationComponent.hpp"
#include "utils/TimeOfDay.hpp"

using namespace hatcher;

namespace
{

ComponentTypeRegisterer<ActionPlanningComponent, EComponentList::Gameplay> actionPlanningRegisterer;
ComponentTypeRegisterer<GrowableComponent, EComponentList::Gameplay> growableRegisterer;
ComponentTypeRegisterer<InventoryComponent, EComponentList::Gameplay> inventoryRegisterer;
ComponentTypeRegisterer<HarvestableComponent, EComponentList::Gameplay> harvestableRegisterer;
ComponentTypeRegisterer<ItemComponent, EComponentList::Gameplay> itemRegisterer;
ComponentTypeRegisterer<LockableComponent, EComponentList::Gameplay> lockableRegisterer;
ComponentTypeRegisterer<Movement2DComponent, EComponentList::Gameplay> movement2DRegisterer;
ComponentTypeRegisterer<NameComponent, EComponentList::Gameplay> nameRegisterer;
ComponentTypeRegisterer<ObstacleComponent, EComponentList::Gameplay> obstacleRegisterer;
ComponentTypeRegisterer<Position2DComponent, EComponentList::Gameplay> position2DRegisterer;
ComponentTypeRegisterer<WorkerComponent, EComponentList::Gameplay> workerRegisterer;

ComponentTypeRegisterer<ItemDisplayComponent, EComponentList::Rendering> itemDisplayRegisterer;
ComponentTypeRegisterer<SelectableComponent, EComponentList::Rendering> selectableRegisterer;
ComponentTypeRegisterer<StaticMeshComponent, EComponentList::Rendering> staticMeshRegisterer;
ComponentTypeRegisterer<SteveAnimationComponent, EComponentList::Rendering> steveAnimationRegisterer;

EntityDescriptorRegisterer Axe{
    EntityDescriptorID::Create("Axe"),
    {
        ItemComponent{
            .type = ItemComponent::Tool,
        },
        LockableComponent{},
        NameComponent{
            .name = "Axe",
        },
    },
    {
        StaticMeshComponent{
            .type = StaticMeshComponent::Axe,
        },
    },
};

EntityDescriptorRegisterer LoggingHut{
    EntityDescriptorID::Create("LoggingHut"),
    {
        NameComponent{
            .name = "Logging Hut",
        },
        ObstacleComponent{
            .area = Box2i(glm::ivec2(-1, -1), glm::ivec2(1, 1)),
        },
        Position2DComponent{},
    },
    {
        SelectableComponent{},
        StaticMeshComponent{
            .type = StaticMeshComponent::Hut,
        },
    },
};

EntityDescriptorRegisterer Melon{
    EntityDescriptorID::Create("Melon"),
    {
        NameComponent{
            .name = "Melon",
        },
        GrowableComponent{
            .maturity = 0.25,
            .growthTime = HoursToTicks(1.f),
        },
        Position2DComponent{},
    },
    {
        StaticMeshComponent{
            .type = StaticMeshComponent::Melon,
        },
    },
};

EntityDescriptorRegisterer Rack{
    EntityDescriptorID::Create("Rack"),
    {
        InventoryComponent{},
        ObstacleComponent{
            .area = Box2i(glm::ivec2(0, 0)),
        },
        Position2DComponent{},
    },
    {
        ItemDisplayComponent{.locations = {}},
        SelectableComponent{},
        StaticMeshComponent{
            .type = StaticMeshComponent::Rack,
        },
    },
};

EntityDescriptorRegisterer Steve{
    EntityDescriptorID::Create("Steve"),
    {
        ActionPlanningComponent{},
        InventoryComponent{},
        Movement2DComponent{},
        NameComponent{
            .name = "Steve",
        },
        Position2DComponent{},
        WorkerComponent{},
    },
    {
        ItemDisplayComponent{},
        SelectableComponent{},
        SteveAnimationComponent{},
    },
};

EntityDescriptorRegisterer Tree{
    EntityDescriptorID::Create("Tree"),
    {
        NameComponent{
            .name = "Tree",
        },
        GrowableComponent{
            .maturity = 0.25,
            .growthTime = HoursToTicks(1.f),
        },
        HarvestableComponent{
            .harvest = EntityDescriptorID::Create("Wood"),
            .amount = 5,
        },
        LockableComponent{},
        ObstacleComponent{
            .area = Box2i(glm::ivec2(0, 0)),
        },
        Position2DComponent{},
    },
    {
        StaticMeshComponent{
            .type = StaticMeshComponent::Tree,
        },
    },
};

EntityDescriptorRegisterer Wood{
    EntityDescriptorID::Create("Wood"),
    {
        ItemComponent{
            .type = ItemComponent::Resource,
        },
        LockableComponent{},
        NameComponent{
            .name = "Wood",
        },
    },
    {
        StaticMeshComponent{
            .type = StaticMeshComponent::Wood,
        },
    },
};

} // namespace
