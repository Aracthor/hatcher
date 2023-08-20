#include "ItemComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, ItemComponent& component)
{
    saveLoader << component.name;
}

namespace
{
ComponentTypeRegisterer<ItemComponent> registerer;
} // namespace
