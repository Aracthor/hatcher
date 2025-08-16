#pragma once

#include <optional>

#include "hatcher/Entity.hpp"

using namespace hatcher;

struct ItemComponent
{
    std::optional<Entity> inventory;
};
