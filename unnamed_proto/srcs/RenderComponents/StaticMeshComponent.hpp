#pragma once

#include "hatcher/basic_types.hpp"

struct StaticMeshComponent
{
    enum Type : hatcher::uint
    {
        Axe,
        Locker,
        Melon,
        Tree,
        Wood,
        COUNT,
    };

    hatcher::uint type;
};
