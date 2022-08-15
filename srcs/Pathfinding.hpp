#pragma once

#include <vector>

#include "hatcher/glm_pure.hpp"

namespace hatcher
{
class ComponentManager;
} // namespace hatcher

using namespace hatcher;

namespace Pathfinding
{

std::vector<glm::vec2> GetPath(const glm::vec2& start, const glm::vec2& end,
                               const ComponentManager* componentManager, float borderOffset);

} // namespace Pathfinding
