#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityDescriptorID.hpp"
#include "hatcher/EntityEgg.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"

#include "AsteroidComponent.hpp"
#include "CollidableComponent.hpp"
#include "PositionComponent.hpp"
#include "Score.hpp"
#include "ScoreGiverComponent.hpp"

using namespace hatcher;

namespace
{
class CollisionUpdater final : public Updater
{
    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        auto asteroidComponents = componentManager->ReadComponents<AsteroidComponent>();
        auto positionComponents = componentManager->ReadComponents<PositionComponent>();
        auto collidableComponents = componentManager->ReadComponents<CollidableComponent>();
        auto scoreGiverComponents = componentManager->ReadComponents<ScoreGiverComponent>();
        Score* score = componentManager->WriteWorldComponent<Score>();
        // O(n^2) algorithm complexity. But for this project-exemple, it is enough.
        for (int i = 0; i < componentManager->Count() - 1; i++)
        {
            bool collided = false;
            const auto& positionComponentA = positionComponents[i];
            const auto& collidableComponentA = collidableComponents[i];
            if (collidableComponentA)
            {
                HATCHER_ASSERT(positionComponentA);
                for (int j = i + 1; j < componentManager->Count() && !collided; j++)
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
                                collided = true;

                                if (scoreGiverComponents[i])
                                    score->points += scoreGiverComponents[i]->points;
                                if (scoreGiverComponents[j])
                                    score->points += scoreGiverComponents[j]->points;

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
