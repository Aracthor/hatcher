#pragma once

#include <vector>

#include "hatcher/glm_pure.hpp"

namespace hatcher
{
class ComponentManager;
} // namespace hatcher

namespace Pathfinding
{

std::vector<glm::vec2> GetPath(const glm::vec2& start, const glm::vec2& end,
                               const hatcher::ComponentManager* componentManager,
                               float borderOffset);

} // namespace Pathfinding
