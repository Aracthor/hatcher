#pragma once

#include <glm/vec2.hpp>
#include <vector>

namespace hatcher
{
class DataLoader;
class DataSaver;
} // namespace hatcher

using namespace hatcher;

struct Movement2DComponent
{
    std::vector<glm::vec2> path; // Reversed : last element is the next step.
};

void operator<<(DataSaver& saver, const Movement2DComponent& component);
void operator>>(DataLoader& loader, Movement2DComponent& component);
