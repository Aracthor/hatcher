#pragma once

#include <vector>

#include "hatcher/Entity.hpp"

namespace hatcher
{
class DataLoader;
class DataSaver;
} // namespace hatcher

using namespace hatcher;

struct InventoryComponent
{
    std::vector<Entity> storage;
};

void operator<<(DataSaver& saver, const InventoryComponent& component);
void operator>>(DataLoader& loader, InventoryComponent& component);
