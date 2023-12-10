#include "hatcher/ComponentRegisterer.hpp"
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
#include "UFOComponent.hpp"
#include "WreckageGeneratorComponent.hpp"

#include "hatcher/Maths/glm_pure.hpp"

using namespace hatcher;

namespace
{
ComponentTypeRegisterer<AsteroidComponent, EComponentList::Gameplay> asteroidRegisterer;
ComponentTypeRegisterer<CollidableComponent, EComponentList::Gameplay> collidableRegisterer;
ComponentTypeRegisterer<LifespanComponent, EComponentList::Gameplay> lifespanRegisterer;
ComponentTypeRegisterer<PlayerComponent, EComponentList::Gameplay> playerRegisterer;
ComponentTypeRegisterer<PositionComponent, EComponentList::Gameplay> positionRegisterer;
ComponentTypeRegisterer<ProjectileComponent, EComponentList::Gameplay> projectileRegisterer;
ComponentTypeRegisterer<ScoreGiverComponent, EComponentList::Gameplay> scoreGiverRegisterer;
ComponentTypeRegisterer<ShooterComponent, EComponentList::Gameplay> shooterRegisterer;
ComponentTypeRegisterer<SubdivisionComponent, EComponentList::Gameplay> subdivisionRegisterer;
ComponentTypeRegisterer<UFOComponent, EComponentList::Gameplay> ufoRegisterer;
ComponentTypeRegisterer<WreckageGeneratorComponent, EComponentList::Gameplay> wreckageRegisterer;

ComponentTypeRegisterer<MeshComponent, EComponentList::Rendering> meshRegisterer;

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
            .scale = 1.f,
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
            .scale = 0.5f,
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
            .scale = 0.25f,
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
            .scale = 1.f,
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
            .scale = 1.f,
        },
    },
};

EntityDescriptorRegisterer UFOBig{
    EntityDescriptorID::Create("UFOBig"),
    {
        CollidableComponent{
            .size = 24.f,
        },
        PositionComponent{},
        ScoreGiverComponent{
            .points = 200,
        },
        ShooterComponent{},
        UFOComponent{
            .aimingAngle = M_PI,
        },
        WreckageGeneratorComponent{
            .WreckageID = EntityDescriptorID::Create("WreckageAsteroid"),
            .count = 5,
        },
    },
    {
        MeshComponent{
            .ID = MeshComponent::UFO,
            .scale = 1.f,
        },
    },
};

EntityDescriptorRegisterer UFOSmall{
    EntityDescriptorID::Create("UFOSmall"),
    {
        CollidableComponent{
            .size = 12.f,
        },
        PositionComponent{},
        ScoreGiverComponent{
            .points = 1000,
        },
        ShooterComponent{},
        UFOComponent{
            .aimingAngle = M_PI / 12.f,
        },
        WreckageGeneratorComponent{
            .WreckageID = EntityDescriptorID::Create("WreckageAsteroid"),
            .count = 5,
        },
    },
    {
        MeshComponent{
            .ID = MeshComponent::UFO,
            .scale = 0.5f,
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
            .scale = 1.f,
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
            .scale = 1.f,
        },
    },
};

} // namespace
