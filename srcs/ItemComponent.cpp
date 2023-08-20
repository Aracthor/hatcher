#include "ItemComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, ItemComponent& component) {}

namespace
{
ComponentTypeRegisterer<ItemComponent> registerer;
} // namespace
