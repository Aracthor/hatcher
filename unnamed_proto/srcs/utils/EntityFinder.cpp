#include "EntityFinder.hpp"

#include <limits>

#include "Components/Position2DComponent.hpp"

#include "hatcher/ComponentManager.hpp"

// TODO find better than this O(n^2) !
Entity FindNearestEntity(const ComponentManager* componentManager, Entity sourceEntity,
                         std::function<bool(const ComponentManager*, Entity entity)> pred)
{
    const auto& positions = componentManager->ReadComponents<Position2DComponent>();
    const glm::vec2 source = positions[sourceEntity]->position;
    float minDistanceSq = std::numeric_limits<float>::max();
    Entity result = Entity::Invalid();
    for (int i = 0; i < componentManager->Count(); i++)
    {
        Entity entity(i);
        if (pred(componentManager, entity))
        {
            const glm::vec2 position = positions[entity]->position;
            const glm::vec2 diff = source - position;
            const float distanceSq = diff.x * diff.x + diff.y * diff.y;
            if (distanceSq < minDistanceSq)
            {
                minDistanceSq = distanceSq;
                result = entity;
            }
        }
    }
    return result;
}
