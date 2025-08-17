#pragma once

#include <optional>

#include "hatcher/Entity.hpp"

using namespace hatcher;

enum class EWork
{
    ChopTree,
};

struct WorkerComponent
{
    std::optional<EWork> workIndex;
    std::optional<Entity> target;
    int workedTicks = 0;
    int workLength = 0;
};
