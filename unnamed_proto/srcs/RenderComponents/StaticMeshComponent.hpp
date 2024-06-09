#pragma once

#include "hatcher/basic_types.hpp"

struct StaticMeshComponent
{
    enum Type : hatcher::uint
    {
        Locker,
        Melon,
        COUNT,
    };

    hatcher::uint type;
};
