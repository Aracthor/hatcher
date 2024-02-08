#include "Components/InventoryComponent.hpp"

#include "hatcher/DataLoader.hpp"
#include "hatcher/DataSaver.hpp"

void operator<<(DataSaver& saver, const InventoryComponent& component)
{
    saver << component.storage;
}

void operator>>(DataLoader& loader, InventoryComponent& component)
{
    loader >> component.storage;
}
