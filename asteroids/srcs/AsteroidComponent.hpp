#pragma once

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

struct AsteroidComponent
{
};

void operator<<(hatcher::ISaveLoader& saveLoader, AsteroidComponent& component);
