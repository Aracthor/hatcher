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
    float angle;
    glm::vec2 speed;
};

void operator<<(ISaveLoader& saveLoader, PositionComponent& component);
