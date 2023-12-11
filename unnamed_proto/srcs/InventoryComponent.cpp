#include "InventoryComponent.hpp"

#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, InventoryComponent& component)
{
    saveLoader << component.storage;
}
