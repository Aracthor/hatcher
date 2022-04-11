#pragma once

#include <glm/vec2.hpp>
#include <vector>

struct Movement2DComponent
{
    glm::vec2 Orientation;
    float Speed;
    std::vector<glm::vec2> Path; // Reversed : last element is the next step.
};
