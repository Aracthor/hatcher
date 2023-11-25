#include "hatcher/ComponentManager.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"

#include "CollidableComponent.hpp"
#include "LifespanComponent.hpp"
#include "PositionComponent.hpp"
#include "WreckageGeneratorComponent.hpp"

using namespace hatcher;

namespace
{
float lerp(float a, float b, float t)
{
    return (1 - t) * a + t * b;
}

class WreckageUpdater final : public Updater
{
    glm::vec2 RandomSpeed(float speedMin, float speedMax)
    {
        const float directionAngle = glm::radians(float(rand() % 360));
        const float speed = lerp(speedMin, speedMax, float(rand() % 1000) / 1000.f);
        return glm::vec2(glm::cos(directionAngle), glm::sin(directionAngle)) * speed;
    }

    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override {}

    void OnDeletedEntity(Entity entity, IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        auto wreckageGeneratorComponent = componentManager->ReadComponents<WreckageGeneratorComponent>()[entity];
        if (wreckageGeneratorComponent)
        {
            for (int i = 0; i < wreckageGeneratorComponent->count; i++)
            {
                const glm::vec2 position = componentManager->ReadComponents<PositionComponent>()[entity]->position;
                const float size = componentManager->ReadComponents<CollidableComponent>()[entity]->size;
                EntityEgg wreckage = entityManager->CreateNewEntity(wreckageGeneratorComponent->WreckageID);
                wreckage.GetComponent<PositionComponent>()->position = position;
                wreckage.GetComponent<PositionComponent>()->angle = glm::radians(float(rand() % 360));
                wreckage.GetComponent<PositionComponent>()->speed = RandomSpeed(0.5f, 1.f);
                wreckage.GetComponent<LifespanComponent>()->duration = rand() % (int)size * 3;
            }
        }
    }
};

UpdaterRegisterer<WreckageUpdater> registerer;

} // namespace
