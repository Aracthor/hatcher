#pragma once

#include <vector>

#include "hatcher/Entity.hpp"

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

struct ShooterComponent
{
    std::vector<hatcher::Entity::IDType> shoots;
};

void operator<<(hatcher::ISaveLoader& saveLoader, ShooterComponent& component);
