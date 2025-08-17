#pragma once

#include <optional>

#include "hatcher/Entity.hpp"

using namespace hatcher;

struct LockableComponent
{
    std::optional<Entity> locker;
};
