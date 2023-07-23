#include "SteveAnimationComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, SteveAnimationComponent& component)
{
    saveLoader << component.rightLegAngle;
    saveLoader << component.rightLegRising;
}

namespace
{
RenderComponentTypeRegisterer<SteveAnimationComponent> registerer;
} // namespace
