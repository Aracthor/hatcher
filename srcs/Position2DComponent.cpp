#include "Position2DComponent.hpp"

#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, Position2DComponent& component)
{
    saveLoader << component.position;
}
