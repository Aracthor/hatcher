#pragma once

#include <string>

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

struct NameComponent
{
    std::string name;
};

void operator<<(hatcher::ISaveLoader& saveLoader, NameComponent& component);
