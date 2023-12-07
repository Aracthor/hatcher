#include "hatcher/EntityDescriptor.hpp"

#include "AsteroidComponent.hpp"
#include "CollidableComponent.hpp"
#include "LifespanComponent.hpp"
#include "MeshComponent.hpp"
#include "PlayerComponent.hpp"
#include "PositionComponent.hpp"
#include "ProjectileComponent.hpp"
#include "ScoreGiverComponent.hpp"
#include "ShooterComponent.hpp"
#include "SubdivisionComponent.hpp"
#include "WreckageGeneratorComponent.hpp"

using namespace hatcher;

EntityDescriptorRegisterer AsteroidBig{
    EntityDescriptorID::Create("AsteroidBig"),
    {
        AsteroidComponent{},
        CollidableComponent{
            .size = 50.f,
        },
        PositionComponent{},
        ScoreGiverComponent{
            .points = 20,
        },
        SubdivisionComponent{
            .SubdivisionID = EntityDescriptorID::Create("AsteroidMedium"),
            .count = 2,
        },
        WreckageGeneratorComponent{
            .WreckageID = EntityDescriptorID::Create("WreckageAsteroid"),
            .count = 5,
        },
    },
    {
        MeshComponent{
            .ID = MeshComponent::Asteroid,
        },
    },
};

EntityDescriptorRegisterer AsteroidMedium{
    EntityDescriptorID::Create("AsteroidMedium"),
    {
        AsteroidComponent{},
        CollidableComponent{
            .size = 25.f,
        },
        PositionComponent{},
        ScoreGiverComponent{
            .points = 50,
        },
        SubdivisionComponent{
            .SubdivisionID = EntityDescriptorID::Create("AsteroidSmall"),
            .count = 2,
        },
        WreckageGeneratorComponent{
            .WreckageID = EntityDescriptorID::Create("WreckageAsteroid"),
            .count = 5,
        },
    },
    {
        MeshComponent{
            .ID = MeshComponent::Asteroid,
        },
    },
};

EntityDescriptorRegisterer AsteroidSmall{
    EntityDescriptorID::Create("AsteroidSmall"),
    {
        AsteroidComponent{},
        CollidableComponent{
            .size = 12.f,
        },
        PositionComponent{},
        ScoreGiverComponent{
            .points = 100,
        },
        WreckageGeneratorComponent{
            .WreckageID = EntityDescriptorID::Create("WreckageAsteroid"),
            .count = 5,
        },
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
        WreckageGeneratorComponent{
            .WreckageID = EntityDescriptorID::Create("WreckageShip"),
            .count = 4,
        },
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
