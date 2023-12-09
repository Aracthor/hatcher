#pragma once

#include "hatcher/Maths/Box.hpp"

struct SelectableComponent
{
    hatcher::Box3f box;
    bool selected;
};
