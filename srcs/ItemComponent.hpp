#pragma once

#include <string>

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

using namespace hatcher;

struct ItemComponent
{
    std::string name;
};

void operator<<(ISaveLoader& saveLoader, ItemComponent& component);
