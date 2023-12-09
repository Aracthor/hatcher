#include "ItemComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"

using namespace hatcher;

namespace
{
ComponentTypeRegisterer<ItemComponent, EComponentList::Gameplay> registerer;
} // namespace
