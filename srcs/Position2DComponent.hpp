#pragma once

#include "hatcher/basic_types.hpp"

#include <glm/vec2.hpp>

struct Position2DComponent
{
public:
    static constexpr hatcher::uint Key = 42; // TODO hash something in constexpr.

    glm::vec2 Position;
};
