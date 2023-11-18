#pragma once

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

using namespace hatcher;

struct AsteroidComponent
{
};

void operator<<(ISaveLoader& saveLoader, AsteroidComponent& component);
