#pragma once

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

struct AsteroidComponent
{
    int subdivision;
};

void operator<<(hatcher::ISaveLoader& saveLoader, AsteroidComponent& component);
