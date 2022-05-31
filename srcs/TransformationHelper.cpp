#include "TransformationHelper.hpp"

#include "hatcher/assert.hpp"

namespace TransformationHelper
{

glm::mat4 ModelFromComponents(const std::optional<Position2DComponent>& position2D,
                              const std::optional<Movement2DComponent>& movement2D)
{
    HATCHER_ASSERT(position2D);
    glm::mat4 modelMatrix = glm::mat4(1.f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position2D->position, 0.f));
    if (movement2D)
    {
        const float angle = glm::orientedAngle(glm::vec2(1.f, 0.f), movement2D->orientation);
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.f, 0.f, 1.f));
    }
    return modelMatrix;
}

} // namespace TransformationHelper
