#include "InventoryComponent.hpp"

#include "hatcher/ComponentLoader.hpp"
#include "hatcher/ComponentSaver.hpp"

void operator<<(ComponentSaver& saver, const InventoryComponent& component)
{
    saver << component.storage;
}

void operator>>(ComponentLoader& loader, InventoryComponent& component)
{
    loader >> component.storage;
}
