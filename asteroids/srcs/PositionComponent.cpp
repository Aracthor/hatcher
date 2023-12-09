#include "PositionComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"

using namespace hatcher;

namespace
{
ComponentTypeRegisterer<PositionComponent, EComponentList::Gameplay> registerer;
} // namespace
