#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityDescriptorID.hpp"
#include "hatcher/EntityEgg.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Maths/Box.hpp"
#include "hatcher/Updater.hpp"
#include "hatcher/WorldSettings.hpp"

#include "Components/AsteroidComponent.hpp"
#include "Components/PositionComponent.hpp"

using namespace hatcher;

namespace
{
class AsteroidCreatorUpdater final : public Updater
{
private:
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        auto asteroidComponents = componentManager->ReadComponents<AsteroidComponent>();
        bool hasAnyAsteroid = false;
        for (int i = 0; i < componentManager->Count(); i++)
        {
            if (asteroidComponents[i])
                hasAnyAsteroid = true;
        }

        if (!hasAnyAsteroid)
        {
            RandomGenerator& random = settings.randomGenerator;
            const Box<2, float> centerBox = {{300, 200}, {500, 400}};
            for (int i = 0; i < 3; i++)
            {
                EntityEgg newEntityEgg = entityManager->CreateNewEntity(EntityDescriptorID::Create("AsteroidBig"));
                auto& positionComponent = newEntityEgg.GetComponent<PositionComponent>();
                do
                {
                    positionComponent->position = {random.RandomInt(0, 800 - 1), random.RandomInt(0, 600 - 1)};
                } while (centerBox.Contains(positionComponent->position));
                positionComponent->angle = random.RandomAngle();
                const float speedMin = 1.f;
                const float speedMax = 2.f;
                positionComponent->speed = random.RandomDirection() * random.RandomFloat(speedMin, speedMax);
            }
        }
    }
};

UpdaterRegisterer<AsteroidCreatorUpdater> registerer;

} // namespace