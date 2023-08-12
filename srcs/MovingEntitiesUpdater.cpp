#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Updater.hpp"
#include "hatcher/glm_pure.hpp"

using namespace hatcher;

namespace
{

class MovingEntitiesUpdater final : public Updater
{
public:
    void Update(ComponentManager* componentManager) override
    {
        ComponentWriter<Position2DComponent> positions = componentManager->WriteComponents<Position2DComponent>();
        ComponentWriter<Movement2DComponent> movements = componentManager->WriteComponents<Movement2DComponent>();

        for (int i = 0; i < componentManager->Count(); i++)
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
                    position2D.orientation = glm::normalize(position2D.position - startPosition);
                }
            }
        }
    }
};

UpdaterRegisterer<MovingEntitiesUpdater> registerer;

} // namespace
