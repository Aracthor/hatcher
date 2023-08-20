#include "InventoryComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, InventoryComponent& component)
{
    saveLoader << component.storage;
}

namespace
{
ComponentTypeRegisterer<InventoryComponent> registerer;
} // namespace
