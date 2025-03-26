#include "hatcher/ComponentManager.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"
#include "hatcher/WorldSettings.hpp"

#include "Components/PositionComponent.hpp"
#include "Components/SubdivisionComponent.hpp"

using namespace hatcher;

namespace
{
class SubdivisionUpdater final : public Updater
{
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentManager* componentManager) override {}

    void OnDeletedEntity(Entity entity, WorldSettings& settings, IEntityManager* entityManager,
                         ComponentManager* componentManager) override
    {
        auto subdivisionComponent = componentManager->ReadComponents<SubdivisionComponent>()[entity];
        auto positionComponent = componentManager->ReadComponents<PositionComponent>()[entity];
        if (subdivisionComponent)
        {
            RandomGenerator& random = settings.randomGenerator;
            HATCHER_ASSERT(positionComponent);
            for (int i = 0; i < subdivisionComponent->count; i++)
            {
                EntityEgg subdivision = entityManager->CreateNewEntity(subdivisionComponent->SubdivisionID);
                subdivision.GetComponent<PositionComponent>() = *positionComponent;
                subdivision.GetComponent<PositionComponent>()->speed +=
                    random.RandomDirection() * random.RandomFloat(1.f, 2.f);
                subdivision.GetComponent<PositionComponent>()->angle = random.RandomAngle();
            }
        }
    }
};

UpdaterRegisterer<SubdivisionUpdater> registerer;

} // namespace
