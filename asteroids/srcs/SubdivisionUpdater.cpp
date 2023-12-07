#include "hatcher/ComponentManager.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"

#include "PositionComponent.hpp"
#include "SubdivisionComponent.hpp"

using namespace hatcher;

namespace
{
float lerp(float a, float b, float t)
{
    return (1 - t) * a + t * b;
}

glm::vec2 RandomSpeed(float speedMin, float speedMax)
{
    const float directionAngle = glm::radians(float(rand() % 360));
    const float speed = lerp(speedMin, speedMax, float(rand() % 1000) / 1000.f);
    return glm::vec2(glm::cos(directionAngle), glm::sin(directionAngle)) * speed;
}

class SubdivisionUpdater final : public Updater
{
    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override {}

    void OnDeletedEntity(Entity entity, IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        auto subdivisionComponent = componentManager->ReadComponents<SubdivisionComponent>()[entity];
        auto positionComponent = componentManager->ReadComponents<PositionComponent>()[entity];
        if (subdivisionComponent)
        {
            HATCHER_ASSERT(positionComponent);
            for (int i = 0; i < subdivisionComponent->count; i++)
            {
                EntityEgg subdivision = entityManager->CreateNewEntity(subdivisionComponent->SubdivisionID);
                subdivision.GetComponent<PositionComponent>() = *positionComponent;
                subdivision.GetComponent<PositionComponent>()->speed += RandomSpeed(1.f, 2.f);
                subdivision.GetComponent<PositionComponent>()->angle = glm::radians(float(rand() % 360));
            }
        }
    }
};

UpdaterRegisterer<SubdivisionUpdater> registerer;

} // namespace
