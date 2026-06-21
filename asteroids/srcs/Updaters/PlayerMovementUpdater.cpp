#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/Maths/Vect.hpp"
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
                    positionComponent->angle += 5.f * M_PI / 180.f;
                if (playerComponent->turningRight)
                    positionComponent->angle -= 5.f * M_PI / 180.f;
                if (playerComponent->accelerating)
                {
                    const Vect2f orientation = {std::cos(positionComponent->angle), std::sin(positionComponent->angle)};
                    positionComponent->speed += orientation * 0.1f;
                }
                positionComponent->speed *= 0.99f;
            }
        }
    }
};

UpdaterRegisterer<PlayerMovementUpdater> registerer;

} // namespace
