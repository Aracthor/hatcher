#include "hatcher/EntityDescriptor.hpp"

#include "InventoryComponent.hpp"
#include "ItemComponent.hpp"
#include "Movement2DComponent.hpp"
#include "NameComponent.hpp"
#include "Position2DComponent.hpp"
#include "SelectableComponent.hpp"
#include "StaticMeshComponent.hpp"
#include "SteveAnimationComponent.hpp"

using namespace hatcher;

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
