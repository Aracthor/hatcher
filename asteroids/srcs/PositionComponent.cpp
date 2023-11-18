#include "PositionComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

using namespace hatcher;

void operator<<(ISaveLoader& saveLoader, PositionComponent& component)
{
    saveLoader << component.position;
    saveLoader << component.angle;
    saveLoader << component.speed;
}

namespace
{
ComponentTypeRegisterer<PositionComponent, EComponentList::Gameplay> registerer;
} // namespace