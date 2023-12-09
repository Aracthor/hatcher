#include "Position2DComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"

using namespace hatcher;

namespace
{
ComponentTypeRegisterer<Position2DComponent, EComponentList::Gameplay> registerer;
} // namespace
