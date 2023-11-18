#pragma once

#include "hatcher/basic_types.hpp"

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

using namespace hatcher;

struct StaticMeshComponent
{
    enum Type : uint
    {
        Locker,
        COUNT,
    };

    uint type;
};

void operator<<(ISaveLoader& saveLoader, StaticMeshComponent& component);
