#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/Updater.hpp"

#include "Components/PositionComponent.hpp"

using namespace hatcher;

namespace
{

class MovingEntitiesUpdater final : public Updater
{
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        auto positionComponents = componentAccessor->WriteComponents<PositionComponent>();
        for (int i = 0; i < componentAccessor->Count(); i++)
        {
            auto& positionComponent = positionComponents[i];
            if (positionComponent)
            {
                HATCHER_ASSERT(positionComponent);
                positionComponent->position += positionComponent->speed;
            }
        }
    }
};

UpdaterRegisterer<MovingEntitiesUpdater> registerer;

} // namespace
