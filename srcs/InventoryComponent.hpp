#pragma once

#include <vector>

#include "hatcher/Entity.hpp"

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

using namespace hatcher;

struct InventoryComponent
{
    std::vector<Entity::IDType> storage;
};

void operator<<(ISaveLoader& saveLoader, InventoryComponent& component);
