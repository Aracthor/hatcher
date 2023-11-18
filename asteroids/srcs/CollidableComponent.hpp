#pragma once

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

struct CollidableComponent
{
    float size;
};

void operator<<(hatcher::ISaveLoader& saveLoader, CollidableComponent& component);
