#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/EntityDescriptor.hpp"

#include "Components/InventoryComponent.hpp"
#include "Components/ItemComponent.hpp"
#include "Components/Movement2DComponent.hpp"
#include "Components/NameComponent.hpp"
#include "Components/Position2DComponent.hpp"
#include "RenderComponents/SelectableComponent.hpp"
#include "RenderComponents/StaticMeshComponent.hpp"
#include "RenderComponents/SteveAnimationComponent.hpp"

using namespace hatcher;

namespace
{

ComponentTypeRegisterer<InventoryComponent, EComponentList::Gameplay> inventoryRegisterer;
ComponentTypeRegisterer<ItemComponent, EComponentList::Gameplay> itemRegisterer;
ComponentTypeRegisterer<Movement2DComponent, EComponentList::Gameplay> movement2DRegisterer;
ComponentTypeRegisterer<NameComponent, EComponentList::Gameplay> nameRegisterer;
ComponentTypeRegisterer<Position2DComponent, EComponentList::Gameplay> position2DRegisterer;

ComponentTypeRegisterer<SelectableComponent, EComponentList::Rendering> selectableRegisterer;
ComponentTypeRegisterer<StaticMeshComponent, EComponentList::Rendering> staticMeshRegisterer;
ComponentTypeRegisterer<SteveAnimationComponent, EComponentList::Rendering> steveAnimationRegisterer;

EntityDescriptorRegisterer EMCard{
    EntityDescriptorID::Create("EMCard"),
    {
        ItemComponent{},
        NameComponent{
            .name = "EM Card",
        },
    },
    {},
};

EntityDescriptorRegisterer Locker{
    EntityDescriptorID::Create("Locker"),
    {
        InventoryComponent{},
        NameComponent{
            .name = "Locker",
        },
        Position2DComponent{
            .position = {},
            .orientation = {1.f, 0.f},
        },
    },
    {
        SelectableComponent{
            .box = {},
            .selected = false,
        },
        StaticMeshComponent{
            .type = StaticMeshComponent::Locker,
        },
    },
};

EntityDescriptorRegisterer Steve{
    EntityDescriptorID::Create("Steve"),
    {
        InventoryComponent{},
        Movement2DComponent{},
        NameComponent{
            .name = "Steve",
        },
        Position2DComponent{
            .position = {},
            .orientation = {1.f, 0.f},
        },
    },
    {
        SelectableComponent{
            .box = {},
            .selected = false,
        },
        SteveAnimationComponent{
            .rightLegAngle = 0.f,
            .rightLegRising = false,
        },
    },
};

} // namespace
