#include "hatcher/ComponentAccessor.hpp"
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
    void Update(IEntityManager* entityManager, ComponentAccessor* componentAccessor) override {}

    void OnDeletedEntity(Entity entity, IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        auto subdivisionComponent = componentAccessor->ReadComponents<SubdivisionComponent>()[entity];
        auto positionComponent = componentAccessor->ReadComponents<PositionComponent>()[entity];
        if (subdivisionComponent)
        {
            RandomDirector* randomDirector = componentAccessor->WriteWorldComponent<RandomDirector>();
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
