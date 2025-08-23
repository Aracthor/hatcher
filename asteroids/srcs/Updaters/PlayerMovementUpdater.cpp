#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/Maths/glm_pure.hpp"
#include "hatcher/Updater.hpp"

#include "Components/PlayerComponent.hpp"
#include "Components/PositionComponent.hpp"

using namespace hatcher;

namespace
{

class PlayerMovementUpdater final : public Updater
{
    void Update(IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        auto positionComponents = componentAccessor->WriteComponents<PositionComponent>();
        auto playerComponents = componentAccessor->ReadComponents<PlayerComponent>();
        for (int i = 0; i < componentAccessor->Count(); i++)
        {
            const auto& playerComponent = playerComponents[i];
            auto& positionComponent = positionComponents[i];
            if (playerComponent)
            {
                HATCHER_ASSERT(positionComponent);
                if (playerComponent->turningLeft)
                    positionComponent->angle += glm::radians(5.f);
                if (playerComponent->turningRight)
                    positionComponent->angle -= glm::radians(5.f);
                if (playerComponent->accelerating)
                {
                    const glm::vec2 orientation = {glm::cos(positionComponent->angle),
                                                   glm::sin(positionComponent->angle)};
                    positionComponent->speed += orientation * 0.1f;
                }
                positionComponent->speed *= 0.99f;
            }
        }
    }
};

UpdaterRegisterer<PlayerMovementUpdater> registerer;

} // namespace
