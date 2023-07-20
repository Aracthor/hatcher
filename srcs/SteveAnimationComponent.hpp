#pragma once

#include "hatcher/Maths/Box.hpp"

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

using namespace hatcher;

struct SteveAnimationComponent
{
    float rightLegAngle;
    bool rightLegRising;
};

void operator<<(ISaveLoader& saveLoader, SteveAnimationComponent& component);
