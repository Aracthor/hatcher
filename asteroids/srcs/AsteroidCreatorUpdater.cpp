#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityDescriptorID.hpp"
#include "hatcher/EntityEgg.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Maths/Box.hpp"
#include "hatcher/Updater.hpp"

#include "AsteroidComponent.hpp"
#include "PositionComponent.hpp"

using namespace hatcher;

namespace
{
float lerp(float a, float b, float t)
{
    return (1 - t) * a + t * b;
}

class AsteroidCreatorUpdater final : public Updater
{
public:
    AsteroidCreatorUpdater()
    {
        // TODO to have our own pseudorandom number generator.
        srandom(::time(nullptr));
    }

private:
    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override
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
            const Box<2, float> centerBox = {{300, 200}, {500, 400}};
            for (int i = 0; i < 4; i++)
            {
                EntityEgg newEntityEgg = entityManager->CreateNewEntity(EntityDescriptorID::Create("Asteroid"));
                auto& positionComponent = newEntityEgg.GetComponent<PositionComponent>();
                do
                {
                    positionComponent->position = glm::vec2(rand() % 800, rand() % 600);
                } while (centerBox.Contains(positionComponent->position));
                positionComponent->angle = glm::radians(float(rand() % 360));
                const float directionAngle = glm::radians(float(rand() % 360));
                const float speedMin = 1.f;
                const float speedMax = 2.f;
                const float speed = lerp(speedMin, speedMax, float(rand() % 1000) / 1000.f);
                positionComponent->speed = glm::vec2(glm::cos(directionAngle), glm::sin(directionAngle)) * speed;
            }
        }
    }
};

UpdaterRegisterer<AsteroidCreatorUpdater> registerer;

} // namespace