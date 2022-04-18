#pragma once

#include <vector>

#include <glm/vec2.hpp>

struct Obstacle2DComponent
{
    std::vector<glm::vec2> corners;
};
