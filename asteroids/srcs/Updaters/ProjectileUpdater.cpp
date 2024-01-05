#include "hatcher/ComponentManager.hpp"
#include "hatcher/Updater.hpp"

#include "Components/ProjectileComponent.hpp"
#include "Components/ShooterComponent.hpp"

#include <algorithm> // std::remove

using namespace hatcher;

namespace
{

class ProjecitleUpdater final : public Updater
{
    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override {}

    void OnDeletedEntity(Entity entity, IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        auto projectileComponents = componentManager->WriteComponents<ProjectileComponent>();
        auto shooterComponents = componentManager->WriteComponents<ShooterComponent>();
        if (projectileComponents[entity])
        {
            const Entity::IDType shooterID = projectileComponents[entity]->shooter;
            if (shooterID != Entity::Invalid().ID())
            {
                auto& shooterComponent = shooterComponents[shooterID];
                HATCHER_ASSERT(shooterComponent);
                auto it = std::remove(shooterComponent->shoots.begin(), shooterComponent->shoots.end(), entity.ID());
                HATCHER_ASSERT(it != shooterComponent->shoots.end());
                shooterComponent->shoots.erase(it, shooterComponent->shoots.end());
            }
        }
        if (shooterComponents[entity])
        {
            for (const Entity::IDType projectileID : shooterComponents[entity]->shoots)
            {
                auto& projectileComponent = projectileComponents[projectileID];
                HATCHER_ASSERT(projectileComponent);
                HATCHER_ASSERT(projectileComponent->shooter != Entity::Invalid().ID());
                projectileComponent->shooter = Entity::Invalid().ID();
            }
        }
    }
};

UpdaterRegisterer<ProjecitleUpdater> registerer;

} // namespace
