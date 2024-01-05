#include "hatcher/ComponentManager.hpp"
#include "hatcher/Updater.hpp"

#include "Components/PositionComponent.hpp"

using namespace hatcher;

namespace
{

class MovingEntitiesUpdater final : public Updater
{
    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        auto positionComponents = componentManager->WriteComponents<PositionComponent>();
        for (int i = 0; i < componentManager->Count(); i++)
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
