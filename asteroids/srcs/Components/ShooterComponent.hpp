#pragma once

#include <vector>

#include "hatcher/Entity.hpp"

namespace hatcher
{
class ComponentLoader;
class ComponentSaver;
} // namespace hatcher

struct ShooterComponent
{
    std::vector<hatcher::Entity::IDType> shoots;
};

void operator<<(hatcher::ComponentSaver& saver, const ShooterComponent& component);
void operator>>(hatcher::ComponentLoader& loader, ShooterComponent& component);
