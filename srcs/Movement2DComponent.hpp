#pragma once

#include <glm/vec2.hpp>
#include <vector>

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

using namespace hatcher;

struct Movement2DComponent
{
    std::vector<glm::vec2> path; // Reversed : last element is the next step.
};

void operator<<(ISaveLoader& saveLoader, Movement2DComponent& component);
