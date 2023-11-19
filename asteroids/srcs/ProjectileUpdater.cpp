#include "hatcher/ComponentManager.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"

#include "ProjectileComponent.hpp"

using namespace hatcher;

namespace
{

class ProjecitleUpdater final : public Updater
{
    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        auto projectileComponents = componentManager->WriteComponents<ProjectileComponent>();
        for (int i = 0; i < componentManager->Count(); i++)
        {
            auto& projectileComponent = projectileComponents[i];
            if (projectileComponent)
            {
                projectileComponent->lifespan -= 1;
                if (projectileComponent->lifespan <= 0.f)
                    entityManager->DeleteEntity(Entity(i));
            }
        }
    }
};

UpdaterRegisterer<ProjecitleUpdater> registerer;

} // namespace
