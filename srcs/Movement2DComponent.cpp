#include "Movement2DComponent.hpp"

#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, Movement2DComponent& component)
{
    saveLoader << component.orientation;
    saveLoader << component.speed;
    saveLoader << component.path;
}
