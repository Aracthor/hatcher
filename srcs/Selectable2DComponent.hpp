#pragma once

#include "hatcher/Maths/Box.hpp"
#include "hatcher/glm_pure.hpp"

using namespace hatcher;

struct Selectable2DComponent
{
    Box3f box;
    bool selected;
};
