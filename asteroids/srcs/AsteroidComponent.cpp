#include "AsteroidComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, AsteroidComponent& component) {}

namespace
{
ComponentTypeRegisterer<AsteroidComponent, EComponentList::Gameplay> registerer;
} // namespace
