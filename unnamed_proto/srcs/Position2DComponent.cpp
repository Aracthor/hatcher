#include "Position2DComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, Position2DComponent& component)
{
    saveLoader << component.position;
    saveLoader << component.orientation;
}

namespace
{
ComponentTypeRegisterer<Position2DComponent, EComponentList::Gameplay> registerer;
} // namespace
