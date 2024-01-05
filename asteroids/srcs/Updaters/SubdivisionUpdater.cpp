#include "hatcher/ComponentManager.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"

#include "Components/PositionComponent.hpp"
#include "Components/SubdivisionComponent.hpp"
#include "WorldComponents/RandomDirector.hpp"

using namespace hatcher;

namespace
{
class SubdivisionUpdater final : public Updater
{
    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override {}

    void OnDeletedEntity(Entity entity, IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        auto subdivisionComponent = componentManager->ReadComponents<SubdivisionComponent>()[entity];
        auto positionComponent = componentManager->ReadComponents<PositionComponent>()[entity];
        if (subdivisionComponent)
        {
            RandomDirector* randomDirector = componentManager->WriteWorldComponent<RandomDirector>();
            HATCHER_ASSERT(positionComponent);
            for (int i = 0; i < subdivisionComponent->count; i++)
            {
                EntityEgg subdivision = entityManager->CreateNewEntity(subdivisionComponent->SubdivisionID);
                subdivision.GetComponent<PositionComponent>() = *positionComponent;
                subdivision.GetComponent<PositionComponent>()->speed += randomDirector->RandomDirection(1.f, 2.f);
                subdivision.GetComponent<PositionComponent>()->angle = randomDirector->RandomAngle();
            }
        }
    }
};

UpdaterRegisterer<SubdivisionUpdater> registerer;

} // namespace
