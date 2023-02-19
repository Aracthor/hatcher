#pragma once

#include "hatcher/Maths/Box.hpp"

using namespace hatcher;

struct Selectable2DComponent
{
    Box3f box;
    bool selected;
};
