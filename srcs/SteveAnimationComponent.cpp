#include "SteveAnimationComponent.hpp"

#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, SteveAnimationComponent& component)
{
    saveLoader << component.rightLegAngle;
    saveLoader << component.rightLegRising;
}
