#pragma once

#include <string>

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

using namespace hatcher;

struct NameComponent
{
    std::string name;
};

void operator<<(ISaveLoader& saveLoader, NameComponent& component);
