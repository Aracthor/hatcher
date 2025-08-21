#pragma once

#include <optional>
#include <string>

#include "hatcher/Entity.hpp"

using namespace hatcher;

struct ItemComponent
{
    enum EType
    {
        Resource,
        Tool,
    };

    EType type;
    std::optional<Entity> inventory;
    int count = 1;
};
