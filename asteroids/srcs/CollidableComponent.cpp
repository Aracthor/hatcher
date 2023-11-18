#include "CollidableComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

using namespace hatcher;

void operator<<(ISaveLoader& saveLoader, CollidableComponent& component)
{
    saveLoader << component.size;
}

namespace
{
ComponentTypeRegisterer<CollidableComponent, EComponentList::Gameplay> registerer;
} // namespace
