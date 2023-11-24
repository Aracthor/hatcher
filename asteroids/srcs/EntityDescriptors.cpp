#include "hatcher/EntityDescriptor.hpp"

#include "AsteroidComponent.hpp"
#include "CollidableComponent.hpp"
#include "LifespanComponent.hpp"
#include "MeshComponent.hpp"
#include "PlayerComponent.hpp"
#include "PositionComponent.hpp"
#include "ProjectileComponent.hpp"
#include "ShooterComponent.hpp"

using namespace hatcher;

EntityDescriptorRegisterer Asteroid{
    EntityDescriptorID::Create("Asteroid"),
    {
        AsteroidComponent{
            .subdivision = 2,
        },
        CollidableComponent{
            .size = 50.f,
        },
        PositionComponent{},
    },
    {
        MeshComponent{
            .ID = MeshComponent::Asteroid,
        },
    },
};

EntityDescriptorRegisterer Player{
    EntityDescriptorID::Create("Player"),
    {
        CollidableComponent{
            .size = 24.f,
        },
        PlayerComponent{
            .turningLeft = false,
            .turningRight = false,
            .accelerating = false,
        },
        PositionComponent{
            .position = {400.f, 300.f},
            .angle = M_PI / 2.f,
            .speed = {0.f, 0.f},
        },
        ShooterComponent{},
    },
    {
        MeshComponent{
            .ID = MeshComponent::Player,
        },
    },
};

EntityDescriptorRegisterer Shoot{
    EntityDescriptorID::Create("Shoot"),
    {
        CollidableComponent{
            .size = 2.f,
        },
        LifespanComponent{
            .duration = 50,
        },
        PositionComponent{},
        ProjectileComponent{},
    },
    {
        MeshComponent{
            .ID = MeshComponent::Shoot,
        },
    },
};

EntityDescriptorRegisterer WreckageAsteroid{
    EntityDescriptorID::Create("WreckageAsteroid"),
    {
        LifespanComponent{},
        PositionComponent{},
    },
    {
        MeshComponent{
            .ID = MeshComponent::WreckageAsteroid,
        },
    },
};
EntityDescriptorRegisterer WreckageShip{
    EntityDescriptorID::Create("WreckageShip"),
    {
        LifespanComponent{},
        PositionComponent{},
    },
    {
        MeshComponent{
            .ID = MeshComponent::WreckageShip,
        },
    },
};
