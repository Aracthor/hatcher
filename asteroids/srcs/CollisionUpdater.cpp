#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityDescriptorID.hpp"
#include "hatcher/EntityEgg.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Maths/glm_pure.hpp"
#include "hatcher/Updater.hpp"

#include "AsteroidComponent.hpp"
#include "CollidableComponent.hpp"
#include "PlayerComponent.hpp"
#include "PositionComponent.hpp"
#include "ProjectileComponent.hpp"
#include "Score.hpp"
#include "ScoreGiverComponent.hpp"

using namespace hatcher;

namespace
{
bool IsPlayerOwnedEntity(ComponentReader<PlayerComponent> playerComponents,
                         ComponentReader<ProjectileComponent> projectileComponents, int index)
{
    if (playerComponents[index])
        return true;

    auto projectile = projectileComponents[index];
    return (projectile && playerComponents[projectile->shooter]);
}

class CollisionUpdater final : public Updater
{
    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        auto asteroidComponents = componentManager->ReadComponents<AsteroidComponent>();
        auto collidableComponents = componentManager->ReadComponents<CollidableComponent>();
        auto positionComponents = componentManager->ReadComponents<PositionComponent>();
        auto playerComponents = componentManager->ReadComponents<PlayerComponent>();
        auto projectileComponents = componentManager->ReadComponents<ProjectileComponent>();
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

                                if (scoreGiverComponents[i] &&
                                    IsPlayerOwnedEntity(playerComponents, projectileComponents, j))
                                    score->points += scoreGiverComponents[i]->points;
                                if (scoreGiverComponents[j] &&
                                    IsPlayerOwnedEntity(playerComponents, projectileComponents, i))
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
