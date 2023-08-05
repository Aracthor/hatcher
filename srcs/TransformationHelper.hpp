#pragma once

#include <optional>

#include "Position2DComponent.hpp"

#include "hatcher/glm_pure.hpp"

namespace TransformationHelper
{

glm::mat4 ModelFromComponents(const std::optional<Position2DComponent>& position2D);

} // namespace TransformationHelper
