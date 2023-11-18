#pragma once

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

using namespace hatcher;

struct CollidableComponent
{
    float size;
};

void operator<<(ISaveLoader& saveLoader, CollidableComponent& component);
