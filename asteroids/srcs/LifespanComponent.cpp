#include "LifespanComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

using namespace hatcher;

void operator<<(ISaveLoader& saveLoader, LifespanComponent& component)
{
    saveLoader << component.duration;
}

namespace
{
ComponentTypeRegisterer<LifespanComponent, EComponentList::Gameplay> registerer;
} // namespace
