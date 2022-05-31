#pragma once

#include <optional>

#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"

#include "hatcher/glm_pure.hpp"

namespace TransformationHelper
{

glm::mat4 ModelFromComponents(const std::optional<Position2DComponent>& position2D,
                              const std::optional<Movement2DComponent>& movement2D);

} // namespace TransformationHelper
