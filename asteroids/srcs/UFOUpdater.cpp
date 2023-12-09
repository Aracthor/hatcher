#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityDescriptorID.hpp"
#include "hatcher/EntityEgg.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Maths/Box.hpp"
#include "hatcher/Updater.hpp"

#include "CollidableComponent.hpp"
#include "PositionComponent.hpp"
#include "ProjectileComponent.hpp"
#include "RandomDirector.hpp"
#include "ShooterComponent.hpp"
#include "UFOComponent.hpp"

using namespace hatcher;

namespace
{
class UFOUpdater final : public Updater
{
    void UpdateUFODirection(RandomDirector* randomDirector, PositionComponent& positionComponent)
    {
        if (randomDirector->RandomInt(1, 100) == 1)
        {
            switch (randomDirector->RandomInt(1, 3))
            {
            case 1:
                positionComponent.speed.y = -2.f;
                break;
            case 2:
                positionComponent.speed.y = 0.f;
                break;
            case 3:
                positionComponent.speed.y = 2.f;
                break;
            default:
                HATCHER_ASSERT(false);
                break;
            }
        }
    }

    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        RandomDirector* randomDirector = componentManager->WriteWorldComponent<RandomDirector>();
        auto ufoComponents = componentManager->ReadComponents<UFOComponent>();
        bool hasAnyUFO = false;
        for (int i = 0; i < componentManager->Count(); i++)
        {
            if (ufoComponents[i])
            {
                auto& positionComponent = componentManager->WriteComponents<PositionComponent>()[i];
                HATCHER_ASSERT(positionComponent);
                UpdateUFODirection(randomDirector, *positionComponent);

                auto collidableComponent = componentManager->ReadComponents<CollidableComponent>()[i];
                auto& shooterComponent = componentManager->WriteComponents<ShooterComponent>()[i];
                if (shooterComponent->shoots.size() < 1 && randomDirector->RandomInt(1, 20) == 1)
                {
                    const glm::vec2 direction = randomDirector->RandomDirection(1.f, 1.f);
                    EntityEgg newProjectile = entityManager->CreateNewEntity(EntityDescriptorID::Create("Shoot"));
                    newProjectile.GetComponent<ProjectileComponent>()->shooter = i;
                    auto& projectilePositionComponent = newProjectile.GetComponent<PositionComponent>();
                    projectilePositionComponent->position =
                        positionComponent->position + direction * (collidableComponent->size + 3);
                    projectilePositionComponent->speed = direction * 8.f;
                    shooterComponent->shoots.push_back(newProjectile.NewEntityID().ID());
                }

                hasAnyUFO = true;
            }
        }

        if (!hasAnyUFO && randomDirector->RandomInt(1, 1000) == 1)
        {
            const bool spawnLeft = randomDirector->RandomBool();
            const float spawnX = spawnLeft ? 0.f : 800.f;
            const float spawnY = randomDirector->RandomFloat(0.f, 600.f);

            const EntityDescriptorID descriptor = randomDirector->RandomBool() ? EntityDescriptorID::Create("UFOBig")
                                                                               : EntityDescriptorID::Create("UFOSmall");
            EntityEgg ufoEgg = entityManager->CreateNewEntity(descriptor);
            auto& positionComponent = ufoEgg.GetComponent<PositionComponent>();
            positionComponent->position = {spawnX, spawnY};
            positionComponent->angle = 0.f;
            positionComponent->speed = spawnLeft ? glm::vec2(2.f, 0.f) : glm::vec2(-2.f, 0.f);
        }
    }
};

UpdaterRegisterer<UFOUpdater> registerer;

} // namespace