#pragma once

#include <vector>

#include "hatcher/Entity.hpp"

namespace hatcher
{
class DataLoader;
class DataSaver;
} // namespace hatcher

struct ShooterComponent
{
    std::vector<hatcher::Entity::IDType> shoots;
};

void operator<<(hatcher::DataSaver& saver, const ShooterComponent& component);
void operator>>(hatcher::DataLoader& loader, ShooterComponent& component);
