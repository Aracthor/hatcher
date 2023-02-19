#pragma once

#include "hatcher/Maths/Box.hpp"

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

using namespace hatcher;

struct Selectable2DComponent
{
    Box3f box;
    bool selected;
};

void operator<<(ISaveLoader& saveLoader, Selectable2DComponent& component);
