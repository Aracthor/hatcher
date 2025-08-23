#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/EntityDescriptorID.hpp"
#include "hatcher/EntityEgg.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Maths/Box.hpp"
#include "hatcher/Updater.hpp"

#include "Components/AsteroidComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "WorldComponents/RandomDirector.hpp"

using namespace hatcher;

namespace
{
class AsteroidCreatorUpdater final : public Updater
{
private:
    void Update(IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        auto asteroidComponents = componentAccessor->ReadComponents<AsteroidComponent>();
        bool hasAnyAsteroid = false;
        for (int i = 0; i < componentAccessor->Count(); i++)
        {
            if (asteroidComponents[i])
                hasAnyAsteroid = true;
        }

        if (!hasAnyAsteroid)
        {
            RandomDirector* randomDirector = componentAccessor->WriteWorldComponent<RandomDirector>();
            const Box<2, float> centerBox = {{300, 200}, {500, 400}};
            for (int i = 0; i < 3; i++)
            {
                EntityEgg newEntityEgg = entityManager->CreateNewEntity(EntityDescriptorID::Create("AsteroidBig"));
                auto& positionComponent = newEntityEgg.GetComponent<PositionComponent>();
                do
                {
                    positionComponent->position = {randomDirector->RandomInt(0, 800 - 1),
                                                   randomDirector->RandomInt(0, 600 - 1)};
                } while (centerBox.Contains(positionComponent->position));
                positionComponent->angle = randomDirector->RandomAngle();
                const float speedMin = 1.f;
                const float speedMax = 2.f;
                positionComponent->speed = randomDirector->RandomDirection(speedMin, speedMax);
            }
        }
    }
};

UpdaterRegisterer<AsteroidCreatorUpdater> registerer;

} // namespace