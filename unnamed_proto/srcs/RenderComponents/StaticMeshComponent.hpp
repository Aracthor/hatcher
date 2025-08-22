#pragma once

#include "hatcher/basic_types.hpp"

struct StaticMeshComponent
{
    enum Type : hatcher::uint
    {
        Axe,
        Hut,
        Melon,
        Rack,
        Tree,
        Wood,
        COUNT,
    };

    hatcher::uint type;
};
