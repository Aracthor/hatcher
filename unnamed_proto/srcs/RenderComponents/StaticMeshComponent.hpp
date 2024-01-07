#pragma once

#include "hatcher/basic_types.hpp"

struct StaticMeshComponent
{
    enum Type : hatcher::uint
    {
        Locker,
        COUNT,
    };

    hatcher::uint type;
};
