#pragma once

#include <optional>

#include "hatcher/Entity.hpp"

using namespace hatcher;

struct ActionPlanningComponent
{
    std::optional<int> currentActionIndex;
    std::optional<Entity> lockedEntity;
};
