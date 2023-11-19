#pragma once

#include "hatcher/Entity.hpp"

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

struct ProjectileComponent
{
    float lifespan;
};

void operator<<(hatcher::ISaveLoader& saveLoader, ProjectileComponent& component);
