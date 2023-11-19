#include "AsteroidComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

using namespace hatcher;

void operator<<(ISaveLoader& saveLoader, AsteroidComponent& component)
{
    saveLoader << component.subdivision;
}

namespace
{
ComponentTypeRegisterer<AsteroidComponent, EComponentList::Gameplay> registerer;
} // namespace
