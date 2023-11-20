#pragma once

#include "hatcher/Entity.hpp"

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

struct ProjectileComponent
{
    hatcher::Entity::IDType shooter;
};

void operator<<(hatcher::ISaveLoader& saveLoader, ProjectileComponent& component);
