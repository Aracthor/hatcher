#pragma once

#include <glm/vec2.hpp>
#include <vector>

namespace hatcher
{
class ComponentLoader;
class ComponentSaver;
} // namespace hatcher

using namespace hatcher;

struct Movement2DComponent
{
    std::vector<glm::vec2> path; // Reversed : last element is the next step.
};

void operator<<(ComponentSaver& saver, const Movement2DComponent& component);
void operator>>(ComponentLoader& loader, Movement2DComponent& component);
