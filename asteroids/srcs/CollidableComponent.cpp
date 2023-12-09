#include "CollidableComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"

using namespace hatcher;

namespace
{
ComponentTypeRegisterer<CollidableComponent, EComponentList::Gameplay> registerer;
} // namespace
