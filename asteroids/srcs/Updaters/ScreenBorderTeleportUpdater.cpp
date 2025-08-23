#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/Maths/glm_pure.hpp"
#include "hatcher/Updater.hpp"

#include "Components/CollidableComponent.hpp"
#include "Components/PositionComponent.hpp"

using namespace hatcher;

namespace
{

class ScreenBorderTeleportUpdater final : public Updater
{
    void Update(IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        // TODO Store it somewhere else ?
        const glm::vec2 screenMin = {0, 0};
        const glm::vec2 screenMax = {800, 600};
        const glm::vec2 screenSize = screenMax - screenMin;

        auto positionComponents = componentAccessor->WriteComponents<PositionComponent>();
        auto collidableComponents = componentAccessor->ReadComponents<CollidableComponent>();
        for (int i = 0; i < componentAccessor->Count(); i++)
        {
            const auto& collidableComponent = collidableComponents[i];
            auto& positionComponent = positionComponents[i];
            if (collidableComponent)
            {
                HATCHER_ASSERT(positionComponent);
                if (positionComponent->position.x + collidableComponent->size < screenMin.x)
                    positionComponent->position.x += screenSize.x + collidableComponent->size * 2;
                if (positionComponent->position.x - collidableComponent->size > screenMax.x)
                    positionComponent->position.x -= screenSize.x + collidableComponent->size * 2;
                if (positionComponent->position.y + collidableComponent->size < screenMin.y)
                    positionComponent->position.y += screenSize.y + collidableComponent->size * 2;
                if (positionComponent->position.y - collidableComponent->size > screenMax.y)
                    positionComponent->position.y -= screenSize.y + collidableComponent->size * 2;
            }
        }
    }
};

UpdaterRegisterer<ScreenBorderTeleportUpdater> registerer;

} // namespace