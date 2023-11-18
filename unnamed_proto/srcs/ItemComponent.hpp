#pragma once

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

using namespace hatcher;

struct ItemComponent
{
};

void operator<<(ISaveLoader& saveLoader, ItemComponent& component);
