#include "MovingEntitiesUpdater.hpp"

#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/glm_pure.hpp"

MovingEntitiesUpdater::MovingEntitiesUpdater() = default;

MovingEntitiesUpdater::~MovingEntitiesUpdater() = default;

void MovingEntitiesUpdater::Update(hatcher::ComponentManager* componentManager)
{
    hatcher::span<std::optional<Position2DComponent>> positions =
        componentManager->GetComponents<Position2DComponent>();
    hatcher::span<std::optional<Movement2DComponent>> movements =
        componentManager->GetComponents<Movement2DComponent>();

    HATCHER_ASSERT(positions.size() == movements.size());
    for (size_t i = 0; i < positions.size(); i++)
    {
        if (movements[i])
        {
            HATCHER_ASSERT(positions[i]);
            const float movementLength = 0.05f;
            Movement2DComponent& movement2D = *movements[i];
            Position2DComponent& position2D = *positions[i];
            if (!movement2D.path.empty())
            {
                const glm::vec2 nextObjective = movement2D.path.back();
                const glm::vec2 direction = (nextObjective - positions[i]->position);
                if (glm::length(direction) < movementLength)
                {
                    position2D.position = nextObjective;
                    movement2D.path.pop_back();
                }
                else
                {
                    position2D.position += glm::normalize(direction) * movementLength;
                }
            }
        }
    }
}
