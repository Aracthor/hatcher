#pragma once

#include "hatcher/basic_types.hpp"

struct StaticMeshComponent
{
    enum Type : hatcher::uint
    {
        Axe,
        Melon,
        Rack,
        Tree,
        Wood,
        COUNT,
    };

    hatcher::uint type;
};
