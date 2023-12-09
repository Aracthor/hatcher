#include "AsteroidComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"

using namespace hatcher;

namespace
{
ComponentTypeRegisterer<AsteroidComponent, EComponentList::Gameplay> registerer;
} // namespace
