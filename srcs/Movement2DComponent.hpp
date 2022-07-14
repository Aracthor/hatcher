#pragma once

#include <glm/vec2.hpp>
#include <vector>

struct Movement2DComponent
{
    glm::vec2 orientation;
    float speed;
    std::vector<glm::vec2> path; // Reversed : last element is the next step.
    float obstacleOffset;
};
