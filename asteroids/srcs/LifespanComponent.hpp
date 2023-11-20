#pragma once

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

struct LifespanComponent
{
    float duration;
};

void operator<<(hatcher::ISaveLoader& saveLoader, LifespanComponent& component);
