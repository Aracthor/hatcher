#pragma once

#include <glm/vec2.hpp>

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

using namespace hatcher;

struct PositionComponent
{
    glm::vec2 position;
    glm::vec2 orientation;
};

void operator<<(ISaveLoader& saveLoader, PositionComponent& component);
