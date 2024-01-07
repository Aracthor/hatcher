#pragma once

#include <vector>

#include "hatcher/Entity.hpp"

namespace hatcher
{
class ComponentLoader;
class ComponentSaver;
} // namespace hatcher

using namespace hatcher;

struct InventoryComponent
{
    std::vector<Entity::IDType> storage;
};

void operator<<(ComponentSaver& saver, const InventoryComponent& component);
void operator>>(ComponentLoader& loader, InventoryComponent& component);
