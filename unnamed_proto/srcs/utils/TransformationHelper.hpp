#pragma once

#include "Components/Position2DComponent.hpp"

#include "hatcher/Maths/glm_pure.hpp"

namespace TransformationHelper
{

glm::mat4 ModelFromComponents(const Position2DComponent& position2D);

} // namespace TransformationHelper
