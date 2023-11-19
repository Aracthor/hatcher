#include "hatcher/ComponentManager.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"

#include "AsteroidComponent.hpp"
#include "CollidableComponent.hpp"
#include "PositionComponent.hpp"

using namespace hatcher;

namespace
{
float lerp(float a, float b, float t)
{
    return (1 - t) * a + t * b;
}

class CollisionUpdater final : public Updater
{
    glm::vec2 RandomSpeed()
    {
        const float directionAngle = glm::radians(float(rand() % 360));
        const float speedMin = 1.f;
        const float speedMax = 2.f;
        const float speed = lerp(speedMin, speedMax, float(rand() % 1000) / 1000.f);
        return glm::vec2(glm::cos(directionAngle), glm::sin(directionAngle)) * speed;
    }

    void SubdivideAsteroid(IEntityManager* entityManager, ComponentManager* componentManager, Entity entity,
                           const AsteroidComponent& asteroid)
    {
        if (asteroid.subdivision > 0)
        {
            // There is a bug here if the new entity induce a resize of component vectors,
            // because we are currently iterating on a span of this vector.
            // TODO fix this, probably by adding those entities in vector at the end of the frame.
            const Entity subdivisionA = entityManager->CloneEntity(entity);
            const Entity subdivisionB = entityManager->CloneEntity(entity);
            auto asteroidComponents = componentManager->WriteComponents<AsteroidComponent>();
            auto collidableComponents = componentManager->WriteComponents<CollidableComponent>();
            auto positionComponents = componentManager->WriteComponents<PositionComponent>();
            asteroidComponents[subdivisionA]->subdivision -= 1;
            asteroidComponents[subdivisionB]->subdivision -= 1;
            collidableComponents[subdivisionA]->size /= 2.f;
            collidableComponents[subdivisionB]->size /= 2.f;
            positionComponents[subdivisionA]->speed += RandomSpeed();
            positionComponents[subdivisionA]->angle = glm::radians(float(rand() % 360));
            positionComponents[subdivisionB]->speed += RandomSpeed();
            positionComponents[subdivisionB]->angle = glm::radians(float(rand() % 360));
        }
    }

    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        auto asteroidComponents = componentManager->ReadComponents<AsteroidComponent>();
        auto positionComponents = componentManager->ReadComponents<PositionComponent>();
        auto collidableComponents = componentManager->ReadComponents<CollidableComponent>();
        // O(n^2) algorithm complexity. But for this project-exemple, it is enough.
        for (int i = 0; i < componentManager->Count() - 1; i++)
        {
            const auto& positionComponentA = positionComponents[i];
            const auto& collidableComponentA = collidableComponents[i];
            if (collidableComponentA)
            {
                HATCHER_ASSERT(positionComponentA);
                for (int j = i + 1; j < componentManager->Count(); j++)
                {
                    const auto& positionComponentB = positionComponents[j];
                    const auto& collidableComponentB = collidableComponents[j];
                    if (collidableComponentB)
                    {
                        HATCHER_ASSERT(positionComponentB);
                        const float distance = glm::length(positionComponentA->position - positionComponentB->position);
                        if (distance < collidableComponentA->size + collidableComponentB->size)
                        {
                            if (!asteroidComponents[i] || !asteroidComponents[j])
                            {
                                if (asteroidComponents[i])
                                    SubdivideAsteroid(entityManager, componentManager, Entity(i),
                                                      *asteroidComponents[i]);
                                if (asteroidComponents[j])
                                    SubdivideAsteroid(entityManager, componentManager, Entity(j),
                                                      *asteroidComponents[j]);
                                entityManager->DeleteEntity(Entity(i));
                                entityManager->DeleteEntity(Entity(j));
                            }
                        }
                    }
                }
            }
        }
    }
};

UpdaterRegisterer<CollisionUpdater> registerer;

} // namespace
