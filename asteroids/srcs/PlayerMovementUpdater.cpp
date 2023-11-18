#include "hatcher/ComponentManager.hpp"
#include "hatcher/Maths/glm_pure.hpp"
#include "hatcher/Updater.hpp"

#include "PlayerComponent.hpp"
#include "PositionComponent.hpp"

using namespace hatcher;

namespace
{

class PlayerMovementUpdater final : public Updater
{
    void Update(ComponentManager* componentManager) override
    {
        auto positionComponents = componentManager->WriteComponents<PositionComponent>();
        auto playerComponents = componentManager->ReadComponents<PlayerComponent>();
        for (int i = 0; i < componentManager->Count(); i++)
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
                positionComponent->position += positionComponent->speed;
            }
        }
    }
};

UpdaterRegisterer<PlayerMovementUpdater> registerer;

} // namespace