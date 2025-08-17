#include "Components/Movement2DComponent.hpp"
#include "Components/Position2DComponent.hpp"

#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/Maths/glm_pure.hpp"
#include "hatcher/Updater.hpp"

using namespace hatcher;

namespace
{

class MovingEntitiesUpdater final : public Updater
{
public:
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        ComponentWriter<Position2DComponent> positions = componentAccessor->WriteComponents<Position2DComponent>();
        ComponentWriter<Movement2DComponent> movements = componentAccessor->WriteComponents<Movement2DComponent>();

        for (int i = 0; i < componentAccessor->Count(); i++)
        {
            if (movements[i])
            {
                HATCHER_ASSERT(positions[i]);
                float movementLength = 0.05f;
                Movement2DComponent& movement2D = *movements[i];
                Position2DComponent& position2D = *positions[i];
                if (!movement2D.path.empty())
                {
                    const glm::vec2 startPosition = position2D.position;
                    while (!movement2D.path.empty() && movementLength > 0.f)
                    {
                        glm::vec2 nextObjective = movement2D.path.back();
                        const glm::vec2 direction = (nextObjective - position2D.position);
                        const float distanceToNextObjective = glm::length(direction);
                        if (distanceToNextObjective <= movementLength)
                        {
                            position2D.position = nextObjective;
                            movement2D.path.pop_back();
                            movementLength -= distanceToNextObjective;
                        }
                        else
                        {
                            position2D.position += glm::normalize(direction) * movementLength;
                            movementLength = 0.f;
                        }
                    }
                    const float distance = glm::length(position2D.position - startPosition);
                    if (distance > 0.001) // micro-steps give an absurd orientation because of floating precision.
                        position2D.orientation = (position2D.position - startPosition) / distance;
                }
            }
        }
    }
};

UpdaterRegisterer<MovingEntitiesUpdater> registerer;

} // namespace
