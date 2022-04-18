#pragma once

#include "hatcher/Maths/Box.hpp"
#include "hatcher/glm_pure.hpp"

struct Selectable2DComponent
{
    hatcher::Box3f box;
    bool selected;
};
