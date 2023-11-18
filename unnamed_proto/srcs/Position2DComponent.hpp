#pragma once

#include <glm/vec2.hpp>

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

using namespace hatcher;

struct Position2DComponent
{
    glm::vec2 position;
    glm::vec2 orientation;
};

void operator<<(ISaveLoader& saveLoader, Position2DComponent& component);
