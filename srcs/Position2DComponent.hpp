#pragma once

#include "hatcher/Component.hpp"

#include <glm/vec2.hpp>

struct Position2DComponent
{
public:
    static constexpr hatcher::uint Key = hatcher::ComponentKeyFromName("Position2DComponent");

    glm::vec2 Position;
};
